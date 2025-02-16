// IngameHud.c
modded class IngameHud
{
    // Existující widgety a proměnné
    protected Widget                            panelMRTsStatBars;

    //Factions
    protected ImageWidget                       FactionErebor;
    protected ImageWidget                       FactionGondor;
    protected ImageWidget                       FactionHarad;
    protected ImageWidget                       FactionIsengard;
    protected ImageWidget                       FactionLorien;
    protected ImageWidget                       FactionWoodlandRealm;
    protected ImageWidget                       FactionMistyMountains;
    protected ImageWidget                       FactionMordor;
    protected ImageWidget                       FactionRivendell;
    protected ImageWidget                       FactionRohan;
    protected ImageWidget                       FactionDolGuldur;


    // Player HUD
    protected ImageWidget                       PlayerHudCircle;
    // Player HUD FACES
    protected ImageWidget                       MordorOrcWarriors;

    protected ImageWidget                       GondorLamedonClansman;

    protected ImageWidget                       PlayerHudName;
    protected TextWidget                        PlayerNameValue;
    protected ImageWidget                       PlayerHealthBar;
    protected ProgressBarWidget                 PlayerHealthProgressBar;
    protected TextWidget                        PlayerHealthProgressBarValue;
    protected ImageWidget                       PlayerExperienceBar;
    protected ProgressBarWidget                 PlayerExperienceProgressBar;
    protected TextWidget                        PlayerExperienceProgressBarValue;
    protected ImageWidget                       PlayerHudlvl;
    protected TextWidget                        PlayerHudlvlValue;

    // Player HUD Faction
    protected ImageWidget                       PlayerHudFaction;
    protected TextWidget                        PlayerTeamValue;
    protected ImageWidget                       FactionDolGuldur1;
    protected ImageWidget                       FactionRohan1;
    protected ImageWidget                       FactionRivendell1;
    protected ImageWidget                       FactionMordor1;
    protected ImageWidget                       FactionMistyMountains1;
    protected ImageWidget                       FactionWoodlandRealm1;
    protected ImageWidget                       FactionLorien1;
    protected ImageWidget                       FactionIsengard1;
    protected ImageWidget                       FactionHarad1;
    protected ImageWidget                       FactionGondor1;
    protected ImageWidget                       FactionErebor1;



    // Target HUD
    protected ImageWidget                       TargetHudCircle;
    // Target HUD FACES
    protected ImageWidget                       MordorOrcWarriors1;

    protected ImageWidget                       GondorLamedonClansman1;



    protected ImageWidget                       TargetHudName;
    protected TextWidget                        TargetNameValue;
    protected ImageWidget                       TargetHealthBar;
    protected ProgressBarWidget                 TargetHealthProgressBar;
    protected TextWidget                        TargetHealthProgressBarValue;
    protected ImageWidget                       TargetHudlvl;
    protected TextWidget                        TargetHudlvlValue;
    // Nový widget pro zobrazení teamu cíle
    protected ImageWidget                      TargetHudFaction;
    protected TextWidget                        TargetTeamValue;



    //PlayerActionBars
     protected ImageWidget                       LeftActionBar;
     protected ButtonWidget                      ButtonLFC01;
     protected ButtonWidget                      ButtonLFC02;
     protected ButtonWidget                      ButtonLFC03;
     protected ButtonWidget                      ButtonLFC04;
     protected ButtonWidget                      ButtonLFC05;
     protected ImageWidget                       RightActionBar;
     protected ButtonWidget                      ButtonRFC01;
     protected ButtonWidget                      ButtonRFC02;
     protected ButtonWidget                      ButtonRFC03;
     protected ButtonWidget                      ButtonRFC04;
     protected ButtonWidget                      ButtonRFC05;

    protected ImageWidget                       UltimaBarMain;
    protected ProgressBarWidget                 UltimaProgressBar;
    protected ButtonWidget                      ButtonUltima;
    protected TextWidget                        TextUltima;
    protected ImageWidget                       UltimaBarMainOverlap;



    
    protected float m_CreatureHealth = 0.0; // HP creatury v HUDu

    // Nové proměnné pro HP creatury
    protected float m_CreatureHealthCurrent = 0.0; // Aktuální HP creatury
    protected float m_CreatureHealthMax = 100.0; // Maximální HP creatury

    protected string m_TargetName = "";
     
    override void Init(Widget hud_panel_widget)
    {
        super.Init(hud_panel_widget);
        Class.CastTo(panelMRTsStatBars, GetGame().GetWorkspace().CreateWidgets("Mefa_creatures/Layouts/mefa_Creatures_hud.layout"));
        HidePlayerHudAndFrames();
        HideTargetHudAndFrames();

        // Inicializace widgetu pro team cíle
        TargetTeamValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetTeamValue"));
    }

    bool IsPlayerControllingCreature()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        return player && player.IsControllingCreature();
    }

    string GetPlayerName()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player && player.GetIdentity())
        {
            return player.GetIdentity().GetName();
        }
        return "Unknown";
    }

    // Metoda pro nastavení HP controllované creatury
    void SetControlledCreatureHealth(float health, float maxHealth) {
        if (maxHealth > 0) {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            EntityAI lvlStone = player.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("MefaCreaturesLevel"));

            float lvlValue = lvlStone.GetQuantity();
            
            m_CreatureHealthCurrent = health; // Uložit aktuální hodnotu HP
            m_CreatureHealthMax = GetMaxHealthForLevel(lvlValue, maxHealth); // Dynamické max HP podle levelu hráče
            //GetGame().ChatPlayer("[Creature] MaxHealth: " + m_CreatureHealthMax.ToString());
        } else {
            m_CreatureHealthCurrent = 100;
            m_CreatureHealthMax = 100;
        }

        UpdateHealthBar();
    }

    float GetMaxHealthForLevel(int playerLevel, float maxHealth) {
		float growthFactor = 1.05; // 5% růst za level
		float multiplier = Math.Pow(growthFactor, playerLevel - 1); // Exponenciální růst
		
		return (maxHealth) * multiplier;
	}

    void UpdateHealthBar() 
    {
        float healthPercentage;
        if (PlayerHealthProgressBar) {
            healthPercentage = (m_CreatureHealthCurrent / m_CreatureHealthMax) * 100.0;
            PlayerHealthProgressBar.SetCurrent(healthPercentage);
        }

        if (PlayerHealthProgressBarValue) {
            // Opravený text: "65 HP (87%)"
            healthPercentage = (m_CreatureHealthCurrent / m_CreatureHealthMax) * 100.0;
            string displayText = string.Format("%1 HP (%2%%)", Math.Round(m_CreatureHealthCurrent), Math.Round(healthPercentage));
            PlayerHealthProgressBarValue.SetText(displayText);

            // Pouze text mění barvu, progress bar zůstává stejný
            int color = ARGB(255, 255, 255, 255); // Výchozí bílá
            if (healthPercentage >= 50) {
                color = ARGB(255, 50, 200, 50);  // Tlumená zelená
            } else if (healthPercentage >= 30) {
                color = ARGB(255, 200, 150, 50); // Tlumená žlutá/oranžová
            } else {
                color = ARGB(255, 180, 50, 50);  // Tlumená červená
            }

            PlayerHealthProgressBar.SetColor(color);
        }
    }

    // --- METODA pro určení aktuálního levelu na základě EXP ---
    int GetCurrentLevel(float expValue, float expThresholds[])
    {
        for (int i = 0; i < 60; i++)
        {
            if (expValue >= expThresholds[i] && expValue < expThresholds[i + 1])
            {
                return i + 1; // Level začíná od 1
            }
        }
        return 60; // Pokud expValue přesáhne všechny hodnoty, nastavíme max level 60
    }

    // --- METODA pro aktualizaci EXP baru ---
    void UpdateExperienceBar()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.GetInventory())
            return;

        // Najdeme položky Level Stone a Experience Stone
        EntityAI lvlStone = player.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("MefaCreaturesLevel"));
        EntityAI expStone = player.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("MefaCreaturesExperiences"));

        if (!expStone || !lvlStone)
        {
            if (PlayerExperienceProgressBar)
                PlayerExperienceProgressBar.SetCurrent(0);
            if (PlayerExperienceProgressBarValue)
                PlayerExperienceProgressBarValue.SetText("EXP: 0");
            if (PlayerHudlvlValue)
                PlayerHudlvlValue.SetText("1"); // Výchozí hodnota pro zobrazení levelu 1
            return;
        }

        float expValue = expStone.GetQuantity();

        // Definujeme rozmezí zkušeností pro každý level
        float expThresholds[61] = { 0, 1000, 2000, 3099, 4308, 5637, 7099, 8706, 10473, 12415, 14551, 16900, 19482, 22322, 25443, 28876, 32650, 36800, 41362, 46379, 51895, 57960, 64628, 71960, 80022, 88886, 98632, 109348, 121130, 134085, 148329, 163990, 181210, 200144, 220962, 243851, 269018, 296690, 327116, 360569, 397352, 437795, 482263, 531155, 584914, 644022, 709012, 780470, 859039, 945427, 1040411, 1144848, 1259678, 1385935, 1524757, 1677393, 1845219, 2029747, 2232637, 2455718, 2700000 }; // Maximum XP pro level 60 

        // Výpočet aktuálního levelu na základě EXP
        int lvlValue = GetCurrentLevel(expValue, expThresholds);

        // Určení hranic EXP pro aktuální level
        float minExp = expThresholds[lvlValue - 1];
        float maxExp = expThresholds[lvlValue];

        // Výpočet procenta XP v rámci aktuálního levelu
        float expPercentage = 0.0;
        if (maxExp > minExp)
        {
            expPercentage = ((expValue - minExp) / (maxExp - minExp)) * 100.0;
        }

        // Ošetření chyb: nikdy nezobrazovat více než 100 % a méně než 0 %
        expPercentage = Math.Clamp(expPercentage, 0.0, 100.0);

        // Zaokrouhlení EXP na celé číslo
        int roundedExp = Math.Round(expValue);

        // Aktualizace HUD
        if (PlayerExperienceProgressBar)
            PlayerExperienceProgressBar.SetCurrent(expPercentage);

        if (PlayerExperienceProgressBarValue)
            PlayerExperienceProgressBarValue.SetText(string.Format("EXP: %1 (%2%%)", roundedExp, Math.Round(expPercentage)));

        if (PlayerHudlvlValue)
        {
            PlayerHudlvlValue.SetText(lvlValue.ToString());
            // Zavolání nového RPC pro aktualizaci množství Level Stone na serveru
            RequestLvlStoneUpdate(lvlValue);
            // Odeslání RPC pro aktualizaci HP kreatury
            RequestCreatureHealthUpdate(lvlValue);
        }
    }

    override void Update(float timeslice)
    {
        super.Update(timeslice);

        Input input = GetGame().GetInput();
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        PlayerIdentity playerIdentity = player.GetIdentity();

        if (IsPlayerControllingCreature())
        {
            ScanForControlledCreature(); // Získání HP creatury
            
            ShowPlayerHudAndFrames(); // Zobraz HUD

            //For testing purpose
            // ShowTargetHudAndFrames();

            FillPlayerControlledCreatureData();

            m_Notifiers.Show(false);
            m_BadgeNotifierDivider.Show(false);
            m_Badges.Show(false);
            m_QuickbarWidget.Show( false );
            if (m_Stamina)
                m_Stamina.Show(false);
            if (m_StaminaBackground)
                m_StaminaBackground.Show(false);


            m_StanceStand.Show(false);
			m_StanceCrouch.Show(false);
			m_StanceProne.Show(false);
			m_StanceCar.Show( false );

             m_PresenceLevel0.Show( false );
			m_PresenceLevel1.Show( false );
			m_PresenceLevel2.Show( false );
			m_PresenceLevel3.Show( false );
			m_PresenceLevel4.Show( false );
            // Aktualizace EXP baru při každém snímku
            UpdateExperienceBar();
            
            if (input.LocalPress("UAMefaTabHold")) { // Hold Tab
                GetGame().ChatPlayer("STOP CONTROLLING ");
            }
            // HideWholeUI();

            // Aktualizace Target HUD:
            // Pokud je ve widgetu TargetNameValue nastaven text (tedy byl již nastaven z RPC), zobrazíme Target HUD.
            // Aktualizace Target HUD: zobrazíme Target HUD pouze, pokud m_TargetName není prázdný
            if (m_TargetName != "" && m_TargetName != "Unknown") {
                ShowTargetHudAndFrames();
            } else {
                HideTargetHudAndFrames();
            }
        }
        else 
        {
            HidePlayerHudAndFrames(); // Skryj HUD
            m_Notifiers.Show(true);
            m_BadgeNotifierDivider.Show(true);
            m_Badges.Show(true);
             m_QuickbarWidget.Show( true );
            if (m_Stamina)
                m_Stamina.Show(true);
            if (m_StaminaBackground)
                m_StaminaBackground.Show(true);


           
            // ShowWholeUI();
        }
    }	

    void ScanForControlledCreature()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.IsAlive() || !player.IsControllingCreature())
            return; // Pokud hráč nesplňuje podmínky, metoda skončí

        vector position = player.GetPosition();
        array<Object> nearbyObjects = new array<Object>();
        GetGame().GetObjectsAtPosition3D(position, 1.0, nearbyObjects, null);
        string playerTeam;
        foreach (Object obj : nearbyObjects)
        {
             mefa_Humanoid_Logic_Base controlledCreatureByPlayer = mefa_Humanoid_Logic_Base.Cast(obj);

             if(controlledCreatureByPlayer)
             {
                 // FACES
                if(controlledCreatureByPlayer.GetType() == "mefa_Orc_Warrior_Controlled")
                {
                    
                    playerTeam = controlledCreatureByPlayer.ConfigGetString("teamName");
                    // GetGame().ChatPlayer("player team: " + playerTeam);
                    SetPlayerFaction(playerTeam);

                    MordorOrcWarriors = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("MordorOrcWarriors"));
                    MordorOrcWarriors.Show(true);
                }
                if(controlledCreatureByPlayer.GetType() == "mefa_Lamedon_Clansmen_Controlled")
                {
                    
                    playerTeam = controlledCreatureByPlayer.ConfigGetString("teamName");
                   // GetGame().ChatPlayer("player team: " + playerTeam);
                    SetPlayerFaction(playerTeam);

                    GondorLamedonClansman = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("GondorLamedonClansman"));
                    GondorLamedonClansman.Show(true);
                }


                if(controlledCreatureByPlayer.GetType().IndexOf("Controlled") != -1)
                {
                    RequestTargetInfo(obj); // Pošli požadavek na server
                    return; // Stačí poslat požadavek jen jednou, metoda skončí
                }
             }
           
        
           
        }
    }

    void RequestTargetInfo(Object target) {
        if (target && GetGame().IsClient()) {
            Param1<Object> rpcData = new Param1<Object>(target);
            GetRPCManager().SendRPC("TargetInfo", "HandleTargetInfoRequest", rpcData, true, null);
        }
    }

    // Nová funkce pro odeslání RPC, která aktualizuje množství Level Stone
    void RequestLvlStoneUpdate(int lvlValue) {
        if (GetGame().IsClient()) {
            Param1<int> rpcData = new Param1<int>(lvlValue);
            GetRPCManager().SendRPC("LvlStoneRPC", "HandleLvlStoneUpdateRequest", rpcData, true, null);
        }
    }

    void RequestCreatureHealthUpdate(int lvlValue) {
        if (GetGame().IsClient()) {
            Param1<int> rpcData = new Param1<int>(lvlValue);
            GetRPCManager().SendRPC("CreatureHealthRPC", "HandleCreatureHealthUpdateRequest", rpcData, true, null);
        }
    }

    // Nová metoda pro nastavení informací o cílové entitě (target) do Target HUD
    void SetTargetCreatureHealth(float health, float maxHealth) {
        if (TargetHealthProgressBar) {
            float healthPercentage;
            if (maxHealth > 0) {
                healthPercentage = (health / maxHealth) * 100.0;
            } else {
                healthPercentage = 0;
            }
            TargetHealthProgressBar.SetCurrent(healthPercentage);
            
            // Nastavení barvy podle procent zdraví:
            int color = ARGB(255, 255, 255, 255); // výchozí bílá
            if (healthPercentage >= 50) {
                color = ARGB(255, 50, 200, 50);  // Tlumená zelená
            } else if (healthPercentage >= 30) {
                color = ARGB(255, 200, 150, 50); // Tlumená žlutá/oranžová
            } else {
                color = ARGB(255, 180, 50, 50);  // Tlumená červená
            }
            TargetHealthProgressBar.SetColor(color);
        }
        if (TargetHealthProgressBarValue) {
            string displayText = string.Format("%1 HP (%2%%)", Math.Round(health), Math.Round((health / maxHealth) * 100));
            TargetHealthProgressBarValue.SetText(displayText);
        }
    }

    void SetTargetCreatureName(string name)
    {
        ImageWidget  MordorOrcWarriors1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("MordorOrcWarriors1"));
        ImageWidget  GondorLamedonClansman1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("GondorLamedonClansman1"));

          // Nejprve skryjeme všechny widgety Faces
        if (MordorOrcWarriors1)           { MordorOrcWarriors1.Show(false); }
        if (GondorLamedonClansman1)        { GondorLamedonClansman1.Show(false); }



        m_TargetName = name; // Uložíme název cíle
        if (TargetNameValue)
        {
            // Pokud chcete připojit i úroveň, můžete to udělat zde – např.:
            // string fullName = name + "  Lvl: " + creatureLevel.ToString();
            // Ale pokud nemáte level přímo, ponechte původní:
            TargetNameValue.SetText(name);
        }
        
        // Příklad kontroly a zobrazení widgetu pro Orc Warriors
        if (name.IndexOf("Orc Warrior") != -1)
        {
             if (MordorOrcWarriors1) { MordorOrcWarriors1.Show(true); }

        }
        else if (name.IndexOf("Lamedon Clansman") != -1)
        {
           if (GondorLamedonClansman1) { GondorLamedonClansman1.Show(true); }
        }
       
    }

    void SetTargetCreatureLevel(int lvl) {
        if (TargetHudlvlValue) {
            TargetHudlvlValue.SetText(lvl.ToString());
        }
    }

    // Nová metoda pro nastavení informace o teamu cíle
    void SetTargetCreatureTeam(string team) {
        if (TargetTeamValue) {
            TargetTeamValue.SetText(team);
        }
    }


  void SetTargetFaction(string team)
    {
        ImageWidget FactionErebor = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionErebor"));
        ImageWidget FactionGondor = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionGondor"));
        ImageWidget FactionHarad = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionHarad"));
        ImageWidget FactionIsengard = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionIsengard"));
        ImageWidget FactionLorien = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionLorien"));
        ImageWidget FactionWoodlandRealm = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionWoodlandRealm"));
        ImageWidget FactionMistyMountains = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionMistyMountains"));
        ImageWidget FactionMordor = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionMordor"));
        ImageWidget FactionRivendell = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionRivendell"));
        ImageWidget FactionRohan = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionRohan"));
        ImageWidget FactionDolGuldur = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionDolGuldur"));
        // Nejprve skryjeme všechny widgety frakcí
        if (FactionErebor)           { FactionErebor.Show(false); }
        if (FactionGondor)           { FactionGondor.Show(false); }
        if (FactionHarad)            { FactionHarad.Show(false); }
        if (FactionIsengard)         { FactionIsengard.Show(false); }
        if (FactionLorien)           { FactionLorien.Show(false); }
        if (FactionWoodlandRealm)    { FactionWoodlandRealm.Show(false); }
        if (FactionMistyMountains)   { FactionMistyMountains.Show(false); }
        if (FactionMordor)           { FactionMordor.Show(false); }
        if (FactionRivendell)        { FactionRivendell.Show(false); }
        if (FactionRohan)            { FactionRohan.Show(false); }
        if (FactionDolGuldur)        { FactionDolGuldur.Show(false); }

        // Podle předaného teamu zobrazíme odpovídající widget
        if (team == "Gondor")
        {
            if (FactionGondor) { FactionGondor.Show(true); }
        }
        else if (team == "Rohan")
        {
            if (FactionRohan) { FactionRohan.Show(true); }
        }
        else if (team == "Mordor")
        {
            if (FactionMordor) { FactionMordor.Show(true); }
        }
        else if (team == "Isengard")
        {
            if (FactionIsengard) { FactionIsengard.Show(true); }
        }
        else if (team == "Lothlorien" || team == "Lorien")
        {
            if (FactionLorien) { FactionLorien.Show(true); }
        }
        else if (team == "Rivendell")
        {
            if (FactionRivendell) { FactionRivendell.Show(true); }
        }
        else if (team == "Erebor")
        {
            if (FactionErebor) { FactionErebor.Show(true); }
        }
        else if (team == "MistyMountains")
        {
            if (FactionMistyMountains) { FactionMistyMountains.Show(true); }
        }
        else if (team == "Haradwaith" || team == "Harad")
        {
            if (FactionHarad) { FactionHarad.Show(true); }
        }
        else if (team == "WoodlandRealm")
        {
            if (FactionWoodlandRealm) { FactionWoodlandRealm.Show(true); }
        }
        else if (team == "DolGuldur")
        {
            if (FactionDolGuldur) { FactionDolGuldur.Show(true); }
        }
    }


    void SetPlayerFaction(string team)
    {
        ImageWidget FactionErebor1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionErebor1"));
        ImageWidget FactionGondor1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionGondor1"));
        ImageWidget FactionHarad1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionHarad1"));
        ImageWidget FactionIsengard1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionIsengard1"));
        ImageWidget FactionLorien1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionLorien1"));
        ImageWidget FactionWoodlandRealm1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionWoodlandRealm1"));
        ImageWidget FactionMistyMountains1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionMistyMountains1"));
        ImageWidget FactionMordor1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionMordor1"));
        ImageWidget FactionRivendell1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionRivendell1"));
        ImageWidget FactionRohan1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionRohan1"));
        ImageWidget FactionDolGuldur1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("FactionDolGuldur1"));
        // Nejprve skryjeme všechny widgety frakcí určené pro hráče
        if (FactionDolGuldur1)      { FactionDolGuldur1.Show(false); }
        if (FactionRohan1)          { FactionRohan1.Show(false); }
        if (FactionRivendell1)       { FactionRivendell1.Show(false); }
        if (FactionMordor1)         { FactionMordor1.Show(false); }
        if (FactionMistyMountains1)  { FactionMistyMountains1.Show(false); }
        if (FactionWoodlandRealm1)   { FactionWoodlandRealm1.Show(false); }
        if (FactionLorien1)         { FactionLorien1.Show(false); }
        if (FactionIsengard1)       { FactionIsengard1.Show(false); }
        if (FactionHarad1)          { FactionHarad1.Show(false); }
        if (FactionGondor1)         { FactionGondor1.Show(false); }
        if (FactionErebor1)         { FactionErebor1.Show(false); }

        // Podle předaného teamu zobrazíme odpovídající widget
        if (team == "Gondor")
        {
            if (FactionGondor1) { FactionGondor1.Show(true); }
        }
        else if (team == "Rohan")
        {
            if (FactionRohan1) { FactionRohan1.Show(true); }
        }
        else if (team == "Mordor")
        {
            if (FactionMordor1) { FactionMordor1.Show(true); }
        }
        else if (team == "Isengard")
        {
            if (FactionIsengard1) { FactionIsengard1.Show(true); }
        }
        else if (team == "Lothlorien" || team == "Lorien")
        {
            if (FactionLorien1) { FactionLorien1.Show(true); }
        }
        else if (team == "Rivendell")
        {
            if (FactionRivendell1) { FactionRivendell1.Show(true); }
        }
        else if (team == "Erebor")
        {
            if (FactionErebor1) { FactionErebor1.Show(true); }
        }
        else if (team == "MistyMountains")
        {
            if (FactionMistyMountains1) { FactionMistyMountains1.Show(true); }
        }
        else if (team == "Haradwaith" || team == "Harad")
        {
            if (FactionHarad1) { FactionHarad1.Show(true); }
        }
        else if (team == "WoodlandRealm")
        {
            if (FactionWoodlandRealm1) { FactionWoodlandRealm1.Show(true); }
        }
    }




    void HideWholeUI()
    {
        m_StancePanel.Show(false);
        Show(false);
    }

    void ShowWholeUI()
    {
        Show(true);
        m_StancePanel.Show(true);
    }

    void FillPlayerControlledCreatureData()
    {
        if (PlayerNameValue)
        {
            PlayerNameValue.SetText(GetPlayerName());
        }
    }

    // GUI - SHOW
    void ShowPlayerHudAndFrames()
    {
        PlayerHudCircle = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHudCircle"));
        PlayerHudCircle.Show(true);
        
        PlayerHudName = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHudName"));
        PlayerHudName.Show(true);
        PlayerNameValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerNameValue"));
        PlayerNameValue.Show(true);
        PlayerHealthBar = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHealthBar"));
        PlayerHealthBar.Show(true);
        Class.CastTo(PlayerHealthProgressBar, panelMRTsStatBars.FindAnyWidget("PlayerHealthProgressBar"));
        PlayerHealthProgressBar = ProgressBarWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHealthProgressBar"));
        PlayerHealthProgressBar.Show(true);
        PlayerHealthProgressBarValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHealthProgressBarValue"));
        PlayerHealthProgressBarValue.Show(true);
        PlayerExperienceBar = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerExperienceBar"));
        PlayerExperienceBar.Show(true);
        PlayerExperienceProgressBar = ProgressBarWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerExperienceProgressBar"));
        PlayerExperienceProgressBar.Show(true);
        PlayerExperienceProgressBarValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerExperienceProgressBarValue"));
        PlayerExperienceProgressBarValue.Show(true);
        PlayerHudlvl = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHudlvl"));
        PlayerHudlvl.Show(true);
        PlayerHudlvlValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHudlvlValue"));
        PlayerHudlvlValue.Show(true);

        PlayerHudFaction = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHudFaction"));
        PlayerHudFaction.Show(true);



        LeftActionBar = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("LeftActionBar"));
        LeftActionBar.Show(true);
        ButtonLFC01 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonLFC01"));
        ButtonLFC01.Show(true);
        ButtonLFC02 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonLFC02"));
        ButtonLFC02.Show(true);
        ButtonLFC03 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonLFC03"));
        ButtonLFC03.Show(true);
        ButtonLFC04 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonLFC04"));
        ButtonLFC04.Show(true);
        ButtonLFC05 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonLFC05"));
        ButtonLFC05.Show(true);

        RightActionBar = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("RightActionBar"));
        RightActionBar.Show(true);
        ButtonRFC01 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonRFC01"));
        ButtonRFC01.Show(true);
        ButtonRFC02 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonRFC02"));
        ButtonRFC02.Show(true);
        ButtonRFC03 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonRFC03"));
        ButtonRFC03.Show(true);
        ButtonRFC04 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonRFC04"));
        ButtonRFC04.Show(true);
        ButtonRFC05 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonRFC05"));
        ButtonRFC05.Show(true);

        UltimaBarMain = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("UltimaBarMain"));
        UltimaBarMain.Show(true);
        ButtonUltima = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonUltima"));
        ButtonUltima.Show(true);
        TextUltima = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("TextUltima"));
        TextUltima.Show(true);
        UltimaProgressBar = ProgressBarWidget.Cast(panelMRTsStatBars.FindAnyWidget("UltimaProgressBar"));
        UltimaProgressBar.Show(true);
        UltimaBarMainOverlap = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("UltimaBarMainOverlap"));
        UltimaBarMainOverlap.Show(true);
    }

    void ShowTargetHudAndFrames()
    {
        TargetHudCircle = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHudCircle"));
        TargetHudCircle.Show(true);
        TargetHudFaction = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHudFaction"));
        TargetHudFaction.Show(true);
        TargetTeamValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetTeamValue"));
        // Zobrazíme také widget s informací o teamu cíle
        /*if (TargetTeamValue) {
            TargetTeamValue.Show(true);
        }*/
        TargetHudName = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHudName"));
        TargetHudName.Show(true);
        TargetNameValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetNameValue"));
        TargetNameValue.Show(true);
        TargetHealthBar = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHealthBar"));
        TargetHealthBar.Show(true);
        Class.CastTo(TargetHealthProgressBar, panelMRTsStatBars.FindAnyWidget("TargetHealthProgressBar"));
        TargetHealthProgressBar = ProgressBarWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHealthProgressBar"));
        TargetHealthProgressBar.Show(true);
        TargetHealthProgressBarValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHealthProgressBarValue"));
        TargetHealthProgressBarValue.Show(true);
        TargetHudlvl = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHudlvl"));
        TargetHudlvl.Show(true);
        TargetHudlvlValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHudlvlValue"));
        TargetHudlvlValue.Show(true);
       
    }

    // GUI - HIDE
    void HidePlayerHudAndFrames()
    {
        PlayerHudCircle = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHudCircle"));
        PlayerHudCircle.Show(false);
        // FACES
        MordorOrcWarriors = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("MordorOrcWarriors"));
        MordorOrcWarriors.Show(false);

        PlayerHudName = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHudName"));
        PlayerHudName.Show(false);
        PlayerNameValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerNameValue"));
        PlayerNameValue.Show(false);
        PlayerHealthBar = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHealthBar"));
        PlayerHealthBar.Show(false);
        Class.CastTo(PlayerHealthProgressBar, panelMRTsStatBars.FindAnyWidget("PlayerHealthProgressBar"));
        PlayerHealthProgressBar = ProgressBarWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHealthProgressBar"));
        PlayerHealthProgressBar.Show(false);
        PlayerHealthProgressBarValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHealthProgressBarValue"));
        PlayerHealthProgressBarValue.Show(false);
        PlayerExperienceBar = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerExperienceBar"));
        PlayerExperienceBar.Show(false);
        Class.CastTo(PlayerExperienceProgressBar, panelMRTsStatBars.FindAnyWidget("PlayerExperienceProgressBar"));
        PlayerExperienceProgressBar = ProgressBarWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerExperienceProgressBar"));
        PlayerExperienceProgressBar.Show(false);
        PlayerExperienceProgressBarValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerExperienceProgressBarValue"));
        PlayerExperienceProgressBarValue.Show(false);
        PlayerHudlvl = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHudlvl"));
        PlayerHudlvl.Show(false);
        PlayerHudlvlValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHudlvlValue"));
        PlayerHudlvlValue.Show(false);


         PlayerHudFaction = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("PlayerHudFaction"));
        PlayerHudFaction.Show(false);


         LeftActionBar = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("LeftActionBar"));
        LeftActionBar.Show(false);
        ButtonLFC01 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonLFC01"));
        ButtonLFC01.Show(false);
        ButtonLFC02 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonLFC02"));
        ButtonLFC02.Show(false);
        ButtonLFC03 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonLFC03"));
        ButtonLFC03.Show(false);
        ButtonLFC04 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonLFC04"));
        ButtonLFC04.Show(false);
        ButtonLFC05 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonLFC05"));
        ButtonLFC05.Show(false);

        RightActionBar = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("RightActionBar"));
        RightActionBar.Show(false);
        ButtonRFC01 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonRFC01"));
        ButtonRFC01.Show(false);
        ButtonRFC02 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonRFC02"));
        ButtonRFC02.Show(false);
        ButtonRFC03 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonRFC03"));
        ButtonRFC03.Show(false);
        ButtonRFC04 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonRFC04"));
        ButtonRFC04.Show(false);
        ButtonRFC05 = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonRFC05"));
        ButtonRFC05.Show(false);

        UltimaBarMain = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("UltimaBarMain"));
        UltimaBarMain.Show(false);
        ButtonUltima = ButtonWidget.Cast(panelMRTsStatBars.FindAnyWidget("ButtonUltima"));
        ButtonUltima.Show(false);
        TextUltima = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("TextUltima"));
        TextUltima.Show(false);
        UltimaProgressBar = ProgressBarWidget.Cast(panelMRTsStatBars.FindAnyWidget("UltimaProgressBar"));
        UltimaProgressBar.Show(false);
        UltimaBarMainOverlap = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("UltimaBarMainOverlap"));
        UltimaBarMainOverlap.Show(false);
    }

    void HideTargetHudAndFrames()
    {
        TargetHudCircle = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHudCircle"));
        TargetHudCircle.Show(false);

        TargetHudFaction = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHudFaction"));
        TargetHudFaction.Show(false);
        TargetTeamValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetTeamValue"));
        // Skryjeme i widget s informací o teamu cíle
        if (TargetTeamValue) {
            TargetTeamValue.Show(false);
        }
        
        // FACES
        //MordorOrcWarriors1 = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("MordorOrcWarriors1"));
        //MordorOrcWarriors1.Show(false);

        TargetHudName = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHudName"));
        TargetHudName.Show(false);
        TargetNameValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetNameValue"));
        TargetNameValue.Show(false);
        TargetHealthBar = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHealthBar"));
        TargetHealthBar.Show(false);
        Class.CastTo(TargetHealthProgressBar, panelMRTsStatBars.FindAnyWidget("TargetHealthProgressBar"));
        TargetHealthProgressBar = ProgressBarWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHealthProgressBar"));
        TargetHealthProgressBar.Show(false);
        TargetHealthProgressBarValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHealthProgressBarValue"));
        TargetHealthProgressBarValue.Show(false);
        TargetHudlvl = ImageWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHudlvl"));
        TargetHudlvl.Show(false);
        TargetHudlvlValue = TextWidget.Cast(panelMRTsStatBars.FindAnyWidget("TargetHudlvlValue"));
        TargetHudlvlValue.Show(false);

    }
};
