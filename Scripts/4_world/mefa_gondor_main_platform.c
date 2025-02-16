class Mefa_Building_Gondor_Fortress_lvl_1 : BaseBuildingBase
{
    private Object Spawned_unit1;
    private Object Spawned_unit2;
    private Object Spawned_unit3;
    private Object Spawned_unit4;

    override bool IsBuilding()
    {
        return false;
    }

    override bool IsWell()
    {
        return true;
    }

    override void SetActions()
    {
        super.SetActions();
    }

    override void EEInit()
    {
        super.EEInit();
        SpawnGondorPlatforms();
    }

    void SpawnGondorPlatforms()
    {
        vector position = this.GetPosition();
        float offset = 5.0; // Adjust this value based on the size of the quarter pieces

        // First quarter
        Spawned_unit1 = GetGame().CreateObject("mefa_gondor_main_platform", position + Vector(35, 0, 0), false, true);
        vector orientation1 = Vector(0, 0, 0);
        Spawned_unit1.SetOrientation(orientation1);

        // Second quarter
        Spawned_unit2 = GetGame().CreateObject("mefa_gondor_main_platform", position + Vector(-35, 0, 0), false, true);
        vector orientation2 = Vector(90, 0, 0);
        Spawned_unit2.SetOrientation(orientation2);

        // Third quarter
        /*Spawned_unit3 = GetGame().CreateObject("mefa_gondor_main_platform", position + Vector(0, 0, 0), false, true);
        vector orientation3 = Vector(0, 0, 180);
        Spawned_unit3.SetOrientation(orientation3);

        // Fourth quarter
        Spawned_unit4 = GetGame().CreateObject("mefa_gondor_main_platform", position + Vector(0, 0, -offset), false, true);
        vector orientation4 = Vector(0, 0, 270);
        Spawned_unit4.SetOrientation(orientation4);*/
    }

    override void SetPartsAfterStoreLoad()
    {
        //update server data
        SetPartsFromSyncData();

        // Kontrola hodnoty "construction_part"
        ConstructionPart construction_part = GetConstruction().GetBaseConstructionPart();

        if (construction_part) {
            SetBaseState(construction_part.IsBuilt());
            SynchronizeBaseState();
        }
    }

    const float MAX_ACTION_DETECTION_DISTANCE = 1.3; // meters
    override bool HasProperDistance(string selection, PlayerBase player)
    {
        if (MemoryPointExists(selection))
        {
            vector selection_pos = ModelToWorld(GetMemoryPointPos(selection));
            float distance = vector.Distance(selection_pos, player.GetPosition());
            if (distance >= MAX_ACTION_DETECTION_DISTANCE)
            {
                return false;
            }
        }
        return true;
    }

    void ~Mefa_Building_Gondor_Fortress_lvl_1()
    {
    }
}
