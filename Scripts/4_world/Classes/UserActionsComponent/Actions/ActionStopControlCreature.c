// Callback třída – slouží k vytvoření akční komponenty (jednorázové použití s množstvím = 1)
class ActionStopControlCreatureCB : ActionSingleUseBaseCB
{
    override void CreateActionComponent()
    {
        // Použijeme jednoduchou komponentu s konstantním "spotřebovaným" množstvím (1)
        m_ActionData.m_ActionComponent = new CASingleUseQuantity(1);
    }
}

// Hlavní akce – umožní hráči kdykoliv ukončit ovládání creatury
class ActionStopControlCreature : ActionSingleUseBase
{
    const int DEFAULT_CONSUMED_QUANTITY = 1;
    
    void ActionStopControlCreature()
    {
        // Nastavení callback třídy
        m_CallbackClass = ActionStopControlCreatureCB;
        // Volíme příkaz podle potřeby – zde ponecháme původní CMD_ACTIONMOD_INTERACTONCE
        m_CommandUID    = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        // Nastavení povolených postojů – zde je použito stejné, jako u konzumace
        m_StanceMask    = DayZPlayerConstants.STANCEIDX_ERECT | DayZPlayerConstants.STANCEIDX_CROUCH;
        // Akce vyžaduje full-body animaci
        m_FullBody      = true;
        // Text akce – ten se zobrazí hráči (můžeš lokalizovat či upravit dle libosti)
        m_Text          = "Stop controlling creature";
    }
    
    // Podmínky – u této akce není potřeba mít žádnou položku (žádný předmět)
    override void CreateConditionComponents()  
    {   
        m_ConditionItem   = new CCINone(); // nevyžaduje se žádná položka
        m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
    }
        
    // Metoda vrací "spotřebované" množství – zde konstantní 1
    int GetConsumedQuantity()
    {
        return DEFAULT_CONSUMED_QUANTITY;
    }
    
    // Podmínka pro spuštění akce: kontroluje, zda cílová entita je ovládaná creatura, kterou hráč právě ovládá
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
         GetGame().ChatPlayer("[ActionStopControlCreature]");
        if (!super.ActionCondition(player, target, item))
            return false;
        
        mefa_Humanoid_Logic_Base controlledEntity = mefa_Humanoid_Logic_Base.Cast(target.GetObject());
        if (!controlledEntity)
            return false;
        
        return controlledEntity.IsControlled() && (controlledEntity.GetControllingPlayer() == player);
    }
    
    // (Nepotřebujeme zde speciální OnEndServer, ale zavoláme super – lze případně rozšířit)
    override void OnEndServer(ActionData action_data)
    {
        super.OnEndServer(action_data);
    }
    
    // Serverová část: zde se provádí logika ukončení ovládání creatury
    override void OnExecuteServer(ActionData action_data)
    {
        mefa_Humanoid_Logic_Base controlledEntity = mefa_Humanoid_Logic_Base.Cast(action_data.m_Target.GetObject());
        PlayerBase player = action_data.m_Player;
        
        if (controlledEntity)
        {
            // Zrušíme stav ovládání na straně creatury
            controlledEntity.SetControlled(false);
            controlledEntity.SetControllingPlayer(null);
            // U hráče nastavíme, že již neovládá žádnou creaturu
            player.SetControllingCreature(false, null);
            
            // Volitelně: odstraníme kontrolní token (pokud je připojen)
           EntityAI token = controlledEntity.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("mefa_ControllToken_Stone"));
            if (token)
            {
               token.Delete();
            }
            
            // Pro informaci vypíšeme zprávu do chatu
            GetGame().ChatPlayer("[ActionStopControlCreature] Control has been stopped.");
        }
    }
}
