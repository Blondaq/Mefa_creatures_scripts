modded class PlayerBase
{
    protected bool m_HiddenPlayer;
	protected bool m_HiddenPlayerClient;

    protected bool m_IsControllingCreature = false; // Stav, zda hráč ovládá stvoření
    protected mefa_Humanoid_Logic_Base m_ControlledCreature; // Reference na ovládané stvoření


    protected ref Timer m_CreatureScanTimer;  // Timer pro pravidelné skenování
    protected float m_ControlledCreatureHealth = 0.0; // Ukládání HP creatury


      // Příznak, zda už byl timer spuštěn
    protected bool m_ExpTimerStarted = false;

    // --- NOVÁ METODA pro získání a nastavení hodnoty Level Stone dle Experience Stone ---
    void PrintExperienceStoneValue()
    {
        if (!GetInventory())
            return;
        if (GetGame().IsClient())
        {
             // Najdeme položku Level Stone v inventáři (název slotu dle konfigurace)
            EntityAI lvlStone = GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("MefaCreaturesLevel"));
            // Najdeme položku Experience Stone v inventáři
            EntityAI expStone = GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("MefaCreaturesExperiences"));

            if (lvlStone)
            {
                float lvlValue = lvlStone.GetQuantity();
               // GetGame().ChatPlayer("Actual Level Stone Value: " + lvlValue.ToString());
            }
            else
            {
                //GetGame().ChatPlayer("Level Stone was not found in inventory!");
            }

            if (expStone && lvlStone)
            {
                float expValue = expStone.GetQuantity();
                //GetGame().ChatPlayer("Experience Stone Value: " + expValue.ToString());

                // Definujeme rozmezí zkušeností pro každý level (stejné jako v HUD)
            float expThresholds[61] = { 0, 1000, 2000, 3099, 4308, 5637, 7099, 8706, 10473, 12415, 14551, 16900, 19482, 22322, 25443, 28876, 32650, 36800, 41362, 46379, 51895, 57960, 64628, 71960, 80022, 88886, 98632, 109348, 121130, 134085, 148329, 163990, 181210, 200144, 220962, 243851, 269018, 296690, 327116, 360569, 397352, 437795, 482263, 531155, 584914, 644022, 709012, 780470, 859039, 945427, 1040411, 1144848, 1259678, 1385935, 1524757, 1677393, 1845219, 2029747, 2232637, 2455718, 2700000}; // Maximum XP pro level 60 

                int playerLevel = 1; // Výchozí level je 1

                // Dynamický výpočet levelu na základě aktuální hodnoty zkušeností
                for (int i = 0; i < 60; i++)
                {
                    if (expValue >= expThresholds[i] && expValue < expThresholds[i + 1])
                    {
                        playerLevel = i + 1; // Level začíná od 1
                        break;
                    }
                }

                // Pokud má hráč plný Experience Stone (2 700 000 XP nebo více), nastavíme level 60
                if (expValue >= 2700000)
                {
                    playerLevel = 60;
                }

                // Nastavení nové hodnoty Level Stone dle spočítané úrovně hráče
                lvlStone.SetQuantity(playerLevel);
               // GetGame().ChatPlayer("Player has actual lvl: " + playerLevel.ToString());
            }
            else
            {
                if (!expStone)
                   // GetGame().ChatPlayer("Experience was not found in inventory!");
            }
        }
       
    }








    // Získání jména hráče ovládajícího creaturu
    string GetControlledCreatureName()
    {
        if (m_ControlledCreature)
        {
            PlayerBase controllingPlayer = m_ControlledCreature.GetControllingPlayer();
            if (controllingPlayer && controllingPlayer.GetIdentity())
            {
                return controllingPlayer.GetIdentity().GetName(); // Vrací jméno hráče
            }
        }
        return "Unknown"; // Pokud není creatura nebo hráč
    }
    

    // Getter pro stav
    bool IsControllingCreature()
    {
        return m_IsControllingCreature;
    }

    // Getter pro referenci na ovládané stvoření
    mefa_Humanoid_Logic_Base GetControlledCreature()
    {
        return m_ControlledCreature;
    }

    // Nastavení stavu a reference
   void SetControllingCreature(bool isControlling, mefa_Humanoid_Logic_Base creature = null)
    {
        m_IsControllingCreature = isControlling;
        m_ControlledCreature = creature;

        // Synchronizace na klienta
        SetSynchDirty();
    }


  

	override void Init()
	{
		super.Init();
        RegisterNetSyncVariableBool("m_HiddenPlayer");
        RegisterNetSyncVariableBool("m_IsControllingCreature");
       
       /* if (GetGame().IsClient() && GetInventory())
        {
            GetInventory().LockInventory(LOCK_FROM_SCRIPT);
            GetGame().GetUIManager().CloseMenu(MENU_INVENTORY);
            GetGame().ChatPlayer("[DEBUG] Inventář byl trvale zamknut!");
        }*/
         // Spustíme pravidelné skenování v okolí
        m_CreatureScanTimer = new Timer(CALL_CATEGORY_SYSTEM);
       // m_CreatureScanTimer.Run(0.5, this, "ScanForControlledCreature", null, true);

        // Podmínka:
        // - pokud nejde o multiplayer (singleplayer), nebo
        // - pokud jde o multiplayer a jsme na serverové instanci (dedikovaný server
        //   nebo u listen serveru instance, která není lokální)
        if ((!GetGame().IsMultiplayer() || GetGame().IsDedicatedServer() || (GetGame().IsServer())) && !m_ExpTimerStarted)
        {
            m_ExpTimerStarted = true;
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.ExpTest, 1000, true);
        }
	}

    // Metoda, která se volá každou sekundu a přidá 500 EXP
    void ExpTest()
    {
         if (m_ControlledCreature && m_ControlledCreature.IsCreatureAlive())
         {
              AddExperience(500);
         }
      
    }

    // Metoda pro přidání EXP do EXP Stone
    void AddExperience(float expAmount)
    {
        // Najdeme položku EXP Stone v inventáři hráče
        EntityAI expStone = GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("MefaCreaturesExperiences"));
        if (!expStone)
        {
           // GetGame().ChatPlayer("EXP Stone nebyl nalezen v inventáři!");
            return;
        }

        // Získáme aktuální hodnotu EXP a přičteme novou hodnotu
        float currentExp = expStone.GetQuantity();
        float newExp = currentExp + expAmount;
        expStone.SetQuantity(newExp);

       // GetGame().ChatPlayer("Přidáno " + expAmount.ToString() + " EXP. Nová EXP hodnota: " + newExp.ToString());
    }
 
  

    // Getter pro HP creatury
    float GetControlledCreatureHealth()
    {
         GetGame().ChatPlayer("GetControlledCreatureHealth: " + m_ControlledCreatureHealth.ToString());
        return m_ControlledCreatureHealth;
    }

    override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
       
        if (source.GetType() == "mefa_Anorien_Archers_Arrow")
        {
            DamageSystem.CloseCombatDamageName(source, this, this.GetHitComponentForAI(), "MeleeFistDummy", this.GetPosition());
            return false;  
        }
        if (source.GetType() == "mefa_Anorien_Archers_Arrow_Fire_Upgrade")
        {
            DamageSystem.CloseCombatDamageName(source, this, this.GetHitComponentForAI(), "MeleeFistDummy", this.GetPosition());
            return false;
        }
        return true;
    }


   





	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		ZombieBase zombie = ZombieBase.Cast(source);
		PlayerBase enemy = PlayerBase.Cast(source.GetHierarchyRootPlayer());
		PlayerBase victim = PlayerBase.Cast(this);
		PlayerBase sourcePlayer;
        float causedDamageValue;
        int objectSize;
        int boneIndex;
        vector localPosition;
        string objectClassName;
        Object objectInstance;
        vector arrowOrientation;
        if (source.GetType() == "mefa_Anorien_Archers_Arrow")
        {
            causedDamageValue = damageResult.GetDamage(dmgZone, "Health");
           // GetGame().ChatPlayer("dmgZone is: " + dmgZone);
          //  GetGame().ChatPlayer("component is: " + component.ToString());
          //  GetGame().ChatPlayer("modelPos is: " + modelPos.ToString());
          //  GetGame().ChatPlayer("damage is: " + causedDamageValue.ToString());


          
             

            objectClassName = "mefa_Anorien_Archers_Arrow";
            localPosition = "0 0 0";
            objectSize = 1;
            boneIndex = this.GetBoneIndexByName(dmgZone);
            if (boneIndex != -1) {
                objectInstance = GetGame().CreateObject(objectClassName, this.GetPosition(), false, true, true);

                // Z�sk�n� orientace ��pu
                arrowOrientation = source.GetOrientation();
               
                objectInstance.SetPosition(localPosition);
                objectInstance.SetOrientation(arrowOrientation);
                this.AddChild(objectInstance, boneIndex);
            }
            else {
                //GetGame().ChatPlayer("Invalid bone: " + dmgZone);
            }

            GetGame().ObjectDelete(source);
        }

        if (source.GetType() == "mefa_Anorien_Archers_Arrow_Fire_Upgrade")
        {
            causedDamageValue = damageResult.GetDamage(dmgZone, "Health");
            // GetGame().ChatPlayer("dmgZone is: " + dmgZone);
           //  GetGame().ChatPlayer("component is: " + component.ToString());
           //  GetGame().ChatPlayer("modelPos is: " + modelPos.ToString());
           //  GetGame().ChatPlayer("damage is: " + causedDamageValue.ToString());





            objectClassName = "mefa_Anorien_Archers_Arrow_Fire_Upgrade";
            localPosition = "0 0 0";
            objectSize = 1;
            boneIndex = this.GetBoneIndexByName(dmgZone);
            if (boneIndex != -1) {
                objectInstance = GetGame().CreateObject(objectClassName, this.GetPosition(), false, true, true);

                // Z�sk�n� orientace ��pu
                arrowOrientation = source.GetOrientation();

                objectInstance.SetPosition(localPosition);
                objectInstance.SetOrientation(arrowOrientation);
                this.AddChild(objectInstance, boneIndex);
            }
            else {
           //     GetGame().ChatPlayer("Invalid bone: " + dmgZone);
            }

            GetGame().ObjectDelete(source);
        }
	}

    // Deklarace nových stavových proměnných
    bool m_LeftMouseClicked = false;

	// Funkce pro vyhledání objektů v dosahu
    void ListNearbyObjects(vector position, float radius)
    {
        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition3D(position, radius, objects, null); // Najdi všechny objekty v daném dosahu

        if (objects.Count() > 0)
        {
            foreach (Object obj : objects)
            {
                // Pokud je nalezen objekt typu "mefa_Orc_Warrior_Controlled"
               
            }
        }
        else
        {
            GetGame().ChatPlayer("No objects found nearby!");
        }
    }


    override void EOnFrame(IEntity other, float timeSlice)
    {
        super.EOnFrame(other, timeSlice);

        if (GetGame().IsClient())
        {
        /* bool isLeftMousePressed = GetGame().GetInput().LocalValue("UAFire") > 0; // Levé tlačítko myši
            bool isRightMousePressed = GetGame().GetInput().LocalValue("UAAim") > 0; // Pravé tlačítko myši
            bool isMiddleMousePressed = GetGame().GetInput().LocalValue("UAZoomIn") > 0; // Prostřední tlačítko myši

            // Levé tlačítko myši
            if (isLeftMousePressed && !m_LeftMouseClicked)
            {
                m_LeftMouseClicked = true;
                ListNearbyObjects(GetPosition(), 10.0); // Logika pro levé tlačítko
            }
            else if (!isLeftMousePressed)
            {
                m_LeftMouseClicked = false;
            }*/

        
        }

        PrintExperienceStoneValue();
    }



    void MutePlayerSounds()
    {
        if (!GetGame().IsServer())
            return;

        // Změníme vrstvu interakce hráče, aby neprodukoval zvuky
        int interactionLayer = dBodyGetInteractionLayer(this);
        interactionLayer &= ~PhxInteractionLayers.CHARACTER; // Odebereme vrstvu "CHARACTER" pro zvuky
        dBodySetInteractionLayer(this, interactionLayer | PhxInteractionLayers.NOCOLLISION);

        // Vypneme animace pohybu hráče (dočasně zmrazíme simulaci)
        DisableSimulation(true);
    }

    void UnmutePlayerSounds()
    {
        if (!GetGame().IsServer())
            return;

        // Obnovíme interakční vrstvu hráče
        dBodySetInteractionLayer(this, PhxInteractionLayers.CHARACTER);

        // Znovu povolíme simulaci animací
        DisableSimulation(false);
    }
        
	void SetPlayerHidden(bool state)
	{
		if (GetGame().IsServer())
		{
			m_HiddenPlayer = state;
			SetSynchDirty();
		}
	}

    override void SetActions() 
	{
		super.SetActions();
		AddAction(ActionControlCreature);
        AddAction(ActionStopControlCreature);
        
       // RemoveAction(ActionUnrestrainSelf);
	}



   





	override bool CanBeTargetedByAI(EntityAI ai)
	{

		return super.CanBeTargetedByAI(ai) && !IsUnconscious();
	}
}