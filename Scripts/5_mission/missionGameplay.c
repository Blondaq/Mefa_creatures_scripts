// MissionGameplay.c
modded class MissionGameplay {
    override void OnMissionStart() {
        super.OnMissionStart();

        // Registrace RPC pro příjem odpovědi od serveru
        GetRPCManager().AddRPC("TargetInfo", "HandleTargetInfoResponse", this, SingeplayerExecutionType.Client);

        // Nová RPC pro target info
        GetRPCManager().AddRPC("TargetInfoTarget", "HandleTargetInfoTargetResponse", this, SingeplayerExecutionType.Client);
    }

    //------------------------------ Health and information for HUD about controlled creature (IngameHud.c (START)) ---------------------------
    // Odeslání požadavku na server
    void RequestTargetInfo(Object target) {
        if (target && GetGame().IsClient()) {
            Param1<Object> rpcData = new Param1<Object>(target);
            GetRPCManager().SendRPC("TargetInfo", "HandleTargetInfoRequest", rpcData, true, null);
        }
    }

    // Zpracování odpovědi od serveru
    void HandleTargetInfoResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target) {
        if (type == CallType.Client) {
            Param2<float, float> data;
            if (!ctx.Read(data)) return;

            float creatureHealth = data.param1;
            float creatureMaxHealth = data.param2;

            IngameHud hud = IngameHud.Cast(GetGame().GetMission().GetHud());
            if (hud) {
                hud.SetControlledCreatureHealth(creatureHealth, creatureMaxHealth);
            }

            // GetGame().ChatPlayer("[Client] Received Health: " + creatureHealth.ToString() + " / " + creatureMaxHealth.ToString());
        }
    }
    //------------------------------ Health and information for HUD about controlled creature (IngameHud.c (END)) ---------------------------

    // Aktualizovaný klientský handler pro target info (Target HUD) s přenosem informace o teamu
    void HandleTargetInfoTargetResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target) {
        if (type == CallType.Client) {
            Param5<float, float, string, int, string> data;
            if (!ctx.Read(data)) return;
            float targetHealth = data.param1;
            float targetMaxHealth = data.param2;
            string targetDisplayName = data.param3;
            int targetLevel = data.param4;
            string targetTeam = data.param5;
            
            IngameHud hud = IngameHud.Cast(GetGame().GetMission().GetHud());
            if (hud) {
                hud.SetTargetCreatureHealth(targetHealth, targetMaxHealth);
                hud.SetTargetCreatureName(targetDisplayName);
                hud.SetTargetCreatureLevel(targetLevel);
                hud.SetTargetFaction(targetTeam);  // Zavoláme novou metodu pro nastavení frakce
            }
        }
    }


    // Pomocná metoda k odeslání RPC požadavku pro target info
    void RequestTargetInfoTarget(Object target) {
        if (target && GetGame().IsClient()) {
            Param1<Object> rpcData = new Param1<Object>(target);
            GetRPCManager().SendRPC("TargetInfoTarget", "HandleTargetInfoTargetRequest", rpcData, true, null);
        }
    }
};
