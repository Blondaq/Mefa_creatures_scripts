class Mefa_Creature_Level_Stone : Inventory_Base
{
	
	PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
	/*ItemBase attachment;
	attachment = ItemBase.Cast(player.GetAttachmentByType(this.Type()));*/

	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);
		player = PlayerBase.Cast(parent.GetHierarchyRootPlayer());
		// GetGame().ChatPlayer("Attached Mefa_Creature_Level_Stone");
		float lvlValue = this.GetQuantity();
		//player.UpdateCreatureHealth(lvlValue);
		if (GetGame().IsServer())
		{
			
		}
	}

	 override void OnQuantityChanged(float delta)
	{
		super.OnQuantityChanged(delta);
		float newQuantity = GetQuantity();
		float oldQuantity = newQuantity - delta; // Odvození starého množství
		if (newQuantity != oldQuantity)
		{
			// Získáme hráče, ke kterému je item připojen
			PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
			if (player)
			{
				// Předpokládáme, že PlayerBase má metodu GetControlledCreature()
				mefa_Humanoid_Logic_Base controlledCreature = player.GetControlledCreature();
				if (controlledCreature)
				{
					// Aktualizujeme HP kreatury podle nového levelu
					controlledCreature.UpdateMaxHealthFromLevel(newQuantity);
				}
			}
		}
	}



	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		super.OnWasDetached(parent, slot_id);
		player = PlayerBase.Cast(parent.GetHierarchyRootPlayer());
	
	}

	void SpawnObject(string objectName, vector position, vector orientation)
	{
		Object obj;
		obj = Object.Cast(GetGame().CreateObject(objectName, "0 0 0"));
		obj.SetPosition(position);
		obj.SetOrientation(orientation);
		//obj.Update();

		// Force update collisions
		if (obj.CanAffectPathgraph())
		{
			obj.SetAffectPathgraph(true, false);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 10, false, obj);
		}
	}

	/*void SpawnPlayerGraveStone(PlayerBase deadPlayer, vector playerDeadPos)
	{
		vector rotation = deadPlayer.GetOrientation();

		//vector orientationForWoodPile = "45 0 0";
		vector newGraveOrientation = rotation;
		vector gravePos = playerDeadPos;
		SpawnObject("Mefa_Player_Gravestone", gravePos, newGraveOrientation);
	}*/


	void ~Mefa_Creature_Level_Stone()
	{
	
	}
};
