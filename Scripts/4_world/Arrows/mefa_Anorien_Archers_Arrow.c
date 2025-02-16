class mefa_Anorien_Archers_Arrow : Inventory_Base
{
	override void EEInit()
	{
		super.EEInit();

		if (GetGame().IsClient()) {
			//GetGame().ChatPlayer("arrow was spawned - client ");
		}

		// Výpis pozice kolize na serveru
		if (GetGame().IsServer()) {
			//GetGame().ChatPlayer("arrow was spawned - server ");
		}

	}

}