class ActionControlCreature: ActionInteractBase
{
   
    PlayerBase m_Player; // Reference na hráče
    EntityAI m_Creature; // Reference na spawnovanou entitu
    

    void ActionControlCreature()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_HUDCursorIcon = CursorIcons.CloseHood;
    }

    override string GetText()
    {
        return "Controll Creature";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        EntityAI controlledEntity;
        Class.CastTo(controlledEntity, target.GetObject());

        if (!controlledEntity)
        {
            return false;
        }


        if (!controlledEntity.IsAlive())
        {
            return false;
        }

       // Kontrola, zda název třídy obsahuje "Controlled"
       if (controlledEntity && controlledEntity.GetType().IndexOf("Controlled") == -1)
        {
            return false;
        }

        

        // Přetypování na mefa_Humanoid_Logic_Base a kontrola, zda není již ovládána
        mefa_Humanoid_Logic_Base controlledLogic = mefa_Humanoid_Logic_Base.Cast(controlledEntity);
        if (controlledLogic && !controlledLogic.IsControlled())
        {
            return true;
        }

        return false;
    }


     override void Start(ActionData action_data)
    {
        super.Start(action_data);
        action_data.m_Player.TryHideItemInHands(true);
        action_data.m_Player.SetPlayerHidden(true);

        if (action_data.m_Player.GetInventory())
        {
            action_data.m_Player.COTSetInvisibility(JMInvisibilityType.DisableSimulation, true);
            action_data.m_Player.PhysicsEnableGravity(false);
            action_data.m_Player.COTSetRemoveCollision(true, true);
        }
    }  
   
     // Pomocná funkce pro umístění pozice na terén
    vector KeepCreatureOnGround(vector position)
    {
        float surfaceY = GetGame().SurfaceY(position[0], position[2]); // Výška terénu
        position[1] = surfaceY; // Nastavení pozice na úroveň terénu
        return position;
    }

 
    override void OnExecuteClient(ActionData action_data)
    {
        Object targetObject = action_data.m_Target.GetObject();
        if (!targetObject) return;

        mefa_Humanoid_Logic_Base controlledEntity = mefa_Humanoid_Logic_Base.Cast(targetObject);
        if (controlledEntity)
        {
            controlledEntity.SetControlled(true);

            CreatureCamera camera = CreatureCamera.Cast(Camera.GetCurrentCamera());
            if (!camera || camera.camTarget != targetObject)
            {
                GetGame().SelectSpectator(action_data.m_Player.GetIdentity(), "CreatureCamera", targetObject.ModelToWorld("0 3 -4"));
                camera = CreatureCamera.Cast(Camera.GetCurrentCamera());
                if (camera) camera.SetTarget(targetObject);
            }

            SetupControl(action_data.m_Player, controlledEntity);

             // 🔒 Zamknutí inventáře a zavření menu
            PlayerBase player = action_data.m_Player;
            /*if (player && player.GetInventory())
            {
                player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);
                GetGame().GetUIManager().CloseMenu(MENU_INVENTORY);
               // GetGame().ChatPlayer("[DEBUG] Inventář byl zamknut!");
            }*/
        }
    }

   override void OnExecuteServer(ActionData action_data)
{
    Object targetObject = action_data.m_Target.GetObject();
    if (!targetObject) return;

    mefa_Humanoid_Logic_Base controlledEntity = mefa_Humanoid_Logic_Base.Cast(targetObject);
    if (controlledEntity)
    {
        PlayerBase player = action_data.m_Player;

        controlledEntity.SetControlled(true);
        controlledEntity.SetControllingPlayer(player);
       /* controlledEntity.SetHealth("GlobalHealth", "Health", 150);
        float creatureHealth = controlledEntity.GetHealth();
        GetGame().ChatPlayer("[ActionControlCreature] Creature Health is: " + creatureHealth.ToString());*/

        // Nastavení hráče, že ovládá entitu
        player.SetControllingCreature(true, controlledEntity);

        // Přidání ControllToken_Stone a přiřazení SteamID
        EntityAI token = EntityAI.Cast(controlledEntity.GetInventory().CreateAttachment("mefa_ControllToken_Stone"));
        mefa_ControllToken_Stone tokenStone = mefa_ControllToken_Stone.Cast(token);
        if (tokenStone)
        {
            string steamID = player.GetIdentity().GetId(); // Získání SteamID hráče
            tokenStone.SetOwnerSteamID(steamID);
             // Automatické svázání hráče
           // RestrainPlayer(player);
            //player.RemoveAction(ActionUnrestrainSelf);
            GetGame().ChatPlayer("[ActionControlCreature] ControllToken_Stone assigned to SteamID: " + steamID);
        }
        else
        {
            GetGame().ChatPlayer("[ActionControlCreature] ERROR: Failed to attach ControllToken_Stone!"); 
        }

       

        SetupControl(player, controlledEntity);
    }
}

    /*void RestrainPlayer(PlayerBase player)
    {
        if (!player)
            return;

        if (!player.IsRestrained()) 
        {
            ItemBase restrainItem = ItemBase.Cast(player.GetItemInHands());

            // Pokud hráč nemá pouta, vytvoříme je
            if (!restrainItem)
            {
                restrainItem = ItemBase.Cast(player.GetInventory().CreateInInventory("Handcuffs"));
                if (!restrainItem)
                {
                    GetGame().ChatPlayer("[RestrainPlayer] ERROR: Nelze vytvořit pouta!");
                    return;
                }
            }

            // Přeměna pout na svázanou verzi
            string new_item_name = MiscGameplayFunctions.ObtainRestrainItemTargetClassname(restrainItem);
            MiscGameplayFunctions.TurnItemIntoItemEx(player, new TurnItemIntoItemLambdaRestrainLambda(restrainItem, new_item_name, player));

            player.SetRestrainStarted(true);
            GetGame().ChatPlayer("[RestrainPlayer] Hráč byl automaticky svázán!");
        }
    }*/

    void SetupControl(PlayerBase player, EntityAI creature)
    {
        m_Player = player;
        m_Creature = creature;
    }

    

}
