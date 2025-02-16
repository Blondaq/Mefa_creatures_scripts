/* MissionServer.c */
modded class MissionServer {
    override void OnInit() {
        super.OnInit();
        
        // Přidání RPC pro příjem dat od klienta
        GetRPCManager().AddRPC("TargetInfo", "HandleTargetInfoRequest", this, SingeplayerExecutionType.Server);
        // Přidání nového RPC pro aktualizaci Level Stone
        GetRPCManager().AddRPC("LvlStoneRPC", "HandleLvlStoneUpdateRequest", this, SingeplayerExecutionType.Server);

        // Registrace RPC v OnInit():
        GetRPCManager().AddRPC("CreatureHealthRPC", "HandleCreatureHealthUpdateRequest", this, SingeplayerExecutionType.Server);

        // Debug zpráva pro potvrzení inicializace RPC
        // WriteToLog("[Server] RPC TargetInfo and LvlStoneRPC initialized.", "INFO");

        // Nová RPC pro target info (Target HUD)
        GetRPCManager().AddRPC("TargetInfoTarget", "HandleTargetInfoTargetRequest", this, SingeplayerExecutionType.Server);
    }

    //------------------------------ Health and information for HUD about controlled creature (IngameHud.c (START)) ---------------------------
    // Zpracování RPC od klienta pro TargetInfo
    void HandleTargetInfoRequest(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target) {
        // WriteToLog("[Server] Received RPC call: HandleTargetInfoRequest");

        if (type == CallType.Server && sender) {
            Param1<Object> data;
            if (!ctx.Read(data)) {
                // WriteToLog("[Server] Failed to read data from RPC context.");
                return;
            }

            Object foundTarget = data.param1;
            float targetHealth = 0.0;
            float targetMaxHealth = 100.0; // Defaultní hodnota
            float soulEnergy;
            float softCapHealth;

            if (foundTarget && foundTarget.IsInherited(ZombieBase)) {
                ZombieBase zombieTarget = ZombieBase.Cast(foundTarget);
                targetHealth = zombieTarget.GetHealth();
                soulEnergy = zombieTarget.GetGame().ConfigGetFloat("CfgVehicles " + zombieTarget.GetType() + " SoulEnergy");
                softCapHealth = zombieTarget.GetGame().ConfigGetFloat("CfgVehicles " + zombieTarget.GetType() + " SoftCapHealth");
                //targetMaxHealth = zombieTarget.GetMaxHealth("", ""); // Získání maximálního zdraví
                targetMaxHealth = soulEnergy + softCapHealth;
                // WriteToLog("[Server] Target is ZombieBase - Health: " + targetHealth.ToString() + ", Max Health: " + targetMaxHealth.ToString(), "INFO");
                // GetGame().ChatPlayer("[Server] Target is ZombieBase - Health: " + targetHealth.ToString() + " / " + targetMaxHealth.ToString());
            } else {
                // WriteToLog("[Server] Target is not a ZombieBase.", "WARNING");
                // GetGame().ChatPlayer("[Server] Target is not a ZombieBase.");
            }

            // Odpověď klientovi s aktuálním a maximálním HP
            Param2<float, float> response = new Param2<float, float>(targetHealth, targetMaxHealth);
            GetRPCManager().SendRPC("TargetInfo", "HandleTargetInfoResponse", response, true, sender);
        }
    }
    //------------------------------ Health and information for HUD about controlled creature (IngameHud.c (END)) ---------------------------

    // Nový RPC handler pro aktualizaci množství Level Stone
      void HandleLvlStoneUpdateRequest(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target) {
        if (type == CallType.Server && sender) {
            Param1<int> data;
            if (!ctx.Read(data)) {
                WriteToLog("[Server] Failed to read lvl value from RPC context.", "WARNING");
                return;
            }
            int newLvlValue = data.param1;

            // Hledáme hráče, jehož identity odpovídá senderu
            array<Man> players = new array<Man>;
            GetGame().GetPlayers(players);
            PlayerBase player = null;
            foreach (Man man : players) {
                PlayerBase pb = PlayerBase.Cast(man);
                if (pb && pb.GetIdentity() && pb.GetIdentity().GetPlainId() == sender.GetPlainId()) {
                    player = pb;
                    break;
                }
            }

            if (player) {
                EntityAI lvlStone = player.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("MefaCreaturesLevel"));
                if (lvlStone && lvlStone.IsInherited(Mefa_Creature_Level_Stone)) {
                    // Aktualizace množství Level Stone na serveru
                    lvlStone.SetQuantity(newLvlValue);
                    WriteToLog("[Server] Updated lvlStone quantity to " + newLvlValue.ToString(), "INFO");
                } else {
                    WriteToLog("[Server] lvlStone not found or not of correct type.", "WARNING");
                }
            } else {
                WriteToLog("[Server] Player not found for sender " + sender.GetPlainId(), "WARNING");
            }
        }
    }


    // Handler:
    void HandleCreatureHealthUpdateRequest(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target) {
        if (type == CallType.Server && sender) {
            Param1<int> data;
            if (!ctx.Read(data)) {
                WriteToLog("[Server] Failed to read level value for creature health update.", "WARNING");
                return;
            }
            int newLvlValue = data.param1;

            // Najdi hráče podle jeho identity (podobně jako dříve)
            array<Man> players = new array<Man>;
            GetGame().GetPlayers(players);
            PlayerBase player = null;
            foreach (Man man : players) {
                PlayerBase pb = PlayerBase.Cast(man);
                if (pb && pb.GetIdentity() && pb.GetIdentity().GetPlainId() == sender.GetPlainId()) {
                    player = pb;
                    break;
                }
            }
            if (!player) {
                WriteToLog("[Server] Player not found for creature health update.", "WARNING");
                return;
            }

            // Předpokládáme, že kontrolovaná kreatura je již nastavena u hráče
            // Například můžeš mít metodu GetControlledCreature() na hráči, která vrací instanci kreatury
            mefa_Humanoid_Logic_Base controlledCreature = player.GetControlledCreature();
            if (controlledCreature) {
                // Aktualizuj level kreatury, což interně zavolá InitHealth() a přepočítá HP
                controlledCreature.SetCreatureLevel(newLvlValue);
                WriteToLog("[Server] Creature HP updated for new player level: " + newLvlValue.ToString(), "INFO");
            } else {
                WriteToLog("[Server] Controlled creature not found.", "WARNING");
            }
        }
    }


    // Handler pro RPC TargetInfoTarget
    void HandleTargetInfoTargetRequest(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target) {
        if (type == CallType.Server && sender) {
            Param1<Object> data;
            if (!ctx.Read(data)) {
                WriteToLog("[Server] Failed to read target object from RPC context.", "WARNING");
                return;
            }
            Object foundTarget = data.param1;
            
            // Pokud je target null, odešleme prázdnou odpověď (pro skrytí Target HUDu)
            if (!foundTarget) {
                Param5<float, float, string, int, string> emptyResponse = new Param5<float, float, string, int, string>(0, 0, "", 0, "");
                GetRPCManager().SendRPC("TargetInfoTarget", "HandleTargetInfoTargetResponse", emptyResponse, true, sender);
                return;
            }
            
            float targetHealth = 0.0;
            float targetMaxHealth = 100.0; // výchozí hodnota
            string targetDisplayName = "Unknown";
            int targetLevel = 1;
            string targetTeam = "";
            
            // Zpracování pro platný cíl...
            if (foundTarget && (foundTarget.IsInherited(ZombieBase) || foundTarget.IsInherited(AnimalBase))) {
                // Pokud cíl dědí z mefa_Humanoid_Logic_Base, použijeme jeho metody
                if (foundTarget.IsInherited(mefa_Humanoid_Logic_Base)) {
                    mefa_Humanoid_Logic_Base creature = mefa_Humanoid_Logic_Base.Cast(foundTarget);
                    targetHealth = creature.GetHealth("", "Health");
                    float baseHealth = creature.softCapHealth + creature.soulEnergy;
                    targetMaxHealth = creature.GetMaxHealthForLevel(creature.CreatureLevel, baseHealth);
                    targetDisplayName = creature.GetDisplayName();
                    targetLevel = creature.CreatureLevel;
                    // Načtení teamu z configu – vytvoříme cestu podle typu objektu
                    targetTeam = creature.ConfigGetString("teamName");

                } else {
                    targetHealth = foundTarget.GetHealth("", "Health");
                    targetMaxHealth = foundTarget.GetMaxHealth("", "Health");
                    targetDisplayName = foundTarget.GetDisplayName();
                }
            }
            
            
            Param5<float, float, string, int, string> response = new Param5<float, float, string, int, string>(targetHealth, targetMaxHealth, targetDisplayName, targetLevel, targetTeam);
            GetRPCManager().SendRPC("TargetInfoTarget", "HandleTargetInfoTargetResponse", response, true, sender);
        }
    }







    void WriteToLog(string message, string level = "INFO") {
        string filePath = "$profile:Mefa_Creatures.log";
        FileHandle logFile = OpenFile(filePath, FileMode.APPEND);
        
        if (logFile != 0) {
            FPrintln(logFile, "[" + level + "] [" + GetDateAndTime() + "] " + message);
            CloseFile(logFile);
        } else {
            Print("[Error] Unable to open log file: " + filePath);
        }
    }

    // Funkce pro získání aktuálního datumu a času
    string GetDateAndTime() {
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        return string.Format("%1-%2-%3 %4:%5:%6", year, month, day, hour, minute, second);
    }
};
