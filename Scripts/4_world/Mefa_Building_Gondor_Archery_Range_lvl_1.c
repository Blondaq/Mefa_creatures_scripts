
class Mefa_Building_Gondor_Archery_Range_lvl_1 : BaseBuildingBase
{
    float timedefer;
    static const float 	UPDATE_TICK_RATE = 10.0;
    float UPDATE_IGNITE_LEVEL_1 = 0.0;
    float UPDATE_IGNITE_LEVEL_2 = 0.0;
    float UPDATE_IGNITE_LEVEL_3 = 0.0;
    ref Timer 			m_TimerUpdate;
    protected Particle 			m_ParticleFire1;
    protected Particle 			m_ParticleFire2;
    protected Particle 			m_ParticleFire3;
    protected Particle 			m_ParticleSmoke;
    protected Particle 			m_ParticleBonfire1;

    void SetupTimerServer()
    {
        if (!m_TimerUpdate)
        {
            m_TimerUpdate = new Timer;
            m_TimerUpdate.Run(UPDATE_TICK_RATE, this, "OnUpdate", null, true);
        }
    }
    void OnUpdate()
    {
        float currentHealth;
        float maxHealth = this.GetMaxHealth("", "health");
        if (GetGame().IsServer())
        {
            currentHealth = this.GetHealth();
        }
        UPDATE_IGNITE_LEVEL_1 = maxHealth * 0.001;
        UPDATE_IGNITE_LEVEL_2 = maxHealth * 0.003;
        UPDATE_IGNITE_LEVEL_3 = maxHealth * 0.005;

        int currentHealthLevel;
        currentHealthLevel = GetHealthLevel();

        if (currentHealthLevel == GameConstants.STATE_PRISTINE)
        {

        }
        if (currentHealthLevel == GameConstants.STATE_WORN)
        {
            this.SetHealth("", "", (currentHealth - UPDATE_IGNITE_LEVEL_1));
        }
        if (currentHealthLevel == GameConstants.STATE_DAMAGED)
        {
            this.SetHealth("", "", (currentHealth - UPDATE_IGNITE_LEVEL_2));
        }
        if (currentHealthLevel == GameConstants.STATE_BADLY_DAMAGED)
        {
            this.SetHealth("", "", (currentHealth - UPDATE_IGNITE_LEVEL_3));
        }
    }


    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);


    }

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
    override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
    {
        super.EEHealthLevelChanged(oldLevel, newLevel, zone);
        int currentHealthLevel;
        currentHealthLevel = GetHealthLevel();

        // IF DAMAGING

        if (oldLevel == GameConstants.STATE_PRISTINE && newLevel == GameConstants.STATE_WORN)
        {
            m_ParticleFire1 = null;
            if (!m_ParticleFire1)
            {
                m_ParticleFire1 = ParticleManager.GetInstance().PlayOnObject(ParticleList.CAMP_NORMAL_FIRE, this, GetMemoryPointPos("firepoint1_position"));
            }
        }

        if (oldLevel == GameConstants.STATE_WORN && newLevel == GameConstants.STATE_DAMAGED)
        {
            m_ParticleFire2 = null;
            if (!m_ParticleFire2)
            {
                m_ParticleFire2 = ParticleManager.GetInstance().PlayOnObject(ParticleList.CAMP_NORMAL_FIRE, this, GetMemoryPointPos("firepoint2_position"));
            }
        }
        if (oldLevel == GameConstants.STATE_DAMAGED && newLevel == GameConstants.STATE_BADLY_DAMAGED)
        {
            m_ParticleFire3 = null;
            if (!m_ParticleFire3)
            {
                m_ParticleFire3 = ParticleManager.GetInstance().PlayOnObject(ParticleList.CAMP_NORMAL_FIRE, this, GetMemoryPointPos("firepoint3_position"));
            }
            m_ParticleSmoke = null;
            if (!m_ParticleSmoke)
            {
                m_ParticleSmoke = ParticleManager.GetInstance().PlayOnObject(ParticleList.BONFIRE_SMOKE, this, GetMemoryPointPos("smokepoint1_position"));
            }
            m_ParticleBonfire1 = null;
            if (!m_ParticleBonfire1)
            {
                m_ParticleBonfire1 = ParticleManager.GetInstance().PlayOnObject(ParticleList.BONFIRE_FIRE, this, GetMemoryPointPos("bonfirepoint1_position"));
            }

        }
        if (oldLevel == GameConstants.STATE_BADLY_DAMAGED && newLevel != GameConstants.STATE_DAMAGED)
        {

            RemoveDamageReceiversInNear();
            RemoveArcheryRangeFeaturesInNear();

            vector archery_ruin_position = this.ModelToWorld(this.GetMemoryPointPos("archery_ruin_position"));
            SpawnObject("StaticObj_Ruin_Rubble", archery_ruin_position, this.GetOrientation());
            GetGame().ObjectDelete(this);

        }
        if (currentHealthLevel == GameConstants.STATE_RUINED)
        {
            if (!m_ParticleBonfire1)
            {
                m_ParticleBonfire1 = ParticleManager.GetInstance().PlayOnObject(ParticleList.BONFIRE_FIRE, this, GetMemoryPointPos("bonfirepoint1_position"));
            }
            // particles
            if (!m_ParticleSmoke)
            {
                m_ParticleSmoke = ParticleManager.GetInstance().PlayOnObject(ParticleList.BONFIRE_SMOKE, this, GetMemoryPointPos("smokepoint1_position"));
            }
            if (m_ParticleFire1)
            {
                m_ParticleFire1.Stop();
            }
            if (m_ParticleFire2)
            {
                m_ParticleFire2.Stop();
            }
            if (m_ParticleFire3)
            {
                m_ParticleFire3.Stop();
            }
        }
    }

    override void EEInit()
    {
        super.EEInit();

        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnDummyDamageReceivers, 5000, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnArcheryRangeFeatures, 3000, false);
        SetupTimerServer();

    }

    void SpawnDummyDamageReceivers()
    {
        if (areDamageReceiversInNear())
        {

        }
        else
        {
             vector position;
             for (int i = 1; i <= 13; i++) {
                 string memoryPoint = "dummyDamageReceiver" + i.ToString() + "_pos";
                 position = this.ModelToWorld(this.GetMemoryPointPos(memoryPoint));
                 SpawnObject("Mefa_Gondor_Archery_Range_Damage_Receiver", position, this.GetOrientation());
             }
        }
    }

    Object Spawned_unit;

    void RefreshUnitPosition()
    {
        vector archer1_pos_refresh = this.ModelToWorld(this.GetMemoryPointPos("archer1_pos"));
        Spawned_unit.SetPosition(archer1_pos_refresh);
    }
    void SpawnArcher01()
    {
        vector archer1_pos = this.ModelToWorld(this.GetMemoryPointPos("archer1_pos"));
      
        Spawned_unit = GetGame().CreateObject("mefa_Gondor_Anorien_archers_dummy", archer1_pos, false, true);

        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(RefreshUnitPosition, 30000, true);
    }

    void SpawnArcher02()
    {
        vector archer2_pos = this.ModelToWorld(this.GetMemoryPointPos("archer2_pos"));
        SpawnUnits(archer2_pos, "mefa_Gondor_Anorien_archers_dummy", false, true);
    }

   void SpawnArcheryRangeFeatures()
   {
       if (areArcheryRangeFeaturesInNear())
       {

       }
       else
       {
          

           GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnArcher01, 4000, false);
          // GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnArcher02, 5000, false);

           vector dummyTarget1_pos = this.ModelToWorld(this.GetMemoryPointPos("dummyTarget1_pos"));
           SpawnObject("Mefa_Gondor_Archery_Range_Dummy_Target", dummyTarget1_pos, this.GetOrientation());

       }
   }

   Object Spawned_creature;
   void SpawnUnits(vector unitSpawnPosition, string creature, bool IsStatic, bool hasBehaviour)
   {
       Spawned_creature = GetGame().CreateObject(creature, unitSpawnPosition, IsStatic, hasBehaviour);
   }

   bool areArcheryRangeFeaturesInNear()
   {
       float radius = 15.0;
       vector checkPosition = this.GetPosition();
       array<Object> nearbyObjects = new array<Object>();
       array<CargoBase> proxyCargos = new array<CargoBase>();

       GetGame().GetObjectsAtPosition3D(checkPosition, radius, nearbyObjects, proxyCargos);
       for (int i = 0; i < nearbyObjects.Count(); i++)
       {
           Object obj = nearbyObjects.Get(i);

           if (obj.IsKindOf("mefa_Gondor_Anorien_archers_dummy") || obj.IsKindOf("Mefa_Gondor_Archery_Range_Dummy_Target"))
           {
               return true;
           }
       }
       delete nearbyObjects;
       delete proxyCargos;
       return false;
   }

   void RemoveArcheryRangeFeaturesInNear()
   {
       float radius = 15.0;
       vector checkPosition = this.GetPosition();
       array<Object> nearbyObjects = new array<Object>();
       array<CargoBase> proxyCargos = new array<CargoBase>();

       GetGame().GetObjectsAtPosition3D(checkPosition, radius, nearbyObjects, proxyCargos);
       for (int i = 0; i < nearbyObjects.Count(); i++)
       {
           Object obj = nearbyObjects.Get(i);

           if (obj.IsKindOf("mefa_Gondor_Anorien_archers_dummy") || obj.IsKindOf("Mefa_Gondor_Archery_Range_Dummy_Target"))
           {
               obj.SetHealth("", "", 0);
               GetGame().ObjectDelete(obj);
           }
       }
       delete nearbyObjects;
       delete proxyCargos;

   }

   bool areDamageReceiversInNear()
   {
       float radius = 15.0;
       vector checkPosition = this.GetPosition();
       array<Object> nearbyObjects = new array<Object>();
       array<CargoBase> proxyCargos = new array<CargoBase>();

       GetGame().GetObjectsAtPosition3D(checkPosition, radius, nearbyObjects, proxyCargos);
       for (int i = 0; i < nearbyObjects.Count(); i++)
       {
           Object obj = nearbyObjects.Get(i);

           if (obj.IsKindOf("Mefa_Gondor_Archery_Range_Damage_Receiver"))
           {
               return true;
           }
       }
       delete nearbyObjects;
       delete proxyCargos;
       return false;
   }

   void RemoveDamageReceiversInNear()
   {
       float radius = 15.0;
       vector checkPosition = this.GetPosition();
       array<Object> nearbyObjects = new array<Object>();
       array<CargoBase> proxyCargos = new array<CargoBase>();

       GetGame().GetObjectsAtPosition3D(checkPosition, radius, nearbyObjects, proxyCargos);
       for (int i = 0; i < nearbyObjects.Count(); i++)
       {
           Object obj = nearbyObjects.Get(i);

           if (obj.IsKindOf("Mefa_Gondor_Archery_Range_Damage_Receiver"))
           {
               obj.SetHealth("", "", 0);
               GetGame().ObjectDelete(obj);
           }
       }
       delete nearbyObjects;
       delete proxyCargos;

   }

    override void SetPartsAfterStoreLoad()
    {
        //update server data
        SetPartsFromSyncData();

        // Kontrola hodnoty "construction_part"
        ConstructionPart construction_part = GetConstruction().GetBaseConstructionPart();
        //Print("construction_part: " + construction_part); // Logování hodnoty construction_part
        if (construction_part) {
            // Pokraèujte dále
            SetBaseState(construction_part.IsBuilt());

            //synchronize after load
            SynchronizeBaseState();
        }
        else {
            //Print("construction_part je null!"); // Toto vám pomùže zjistit, zda je problém s nulovou hodnotou.
        }
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
    

    const float MAX_ACTION_DETECTION_DISTANCE = 1.3;        //meters
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

    void ParticleStopper()
    {
        RemoveDamageReceiversInNear();
        RemoveArcheryRangeFeaturesInNear();
        if (m_ParticleFire1)
        {
            m_ParticleFire1.Stop();
        }
        if (m_ParticleFire2)
        {
            m_ParticleFire2.Stop();
        }
        if (m_ParticleFire3)
        {
            m_ParticleFire3.Stop();
        }
    };

    void ~Mefa_Building_Gondor_Archery_Range_lvl_1()
    {
        if (m_TimerUpdate)
        {
            m_TimerUpdate.Stop();

            m_TimerUpdate = null;

        }
        ParticleStopper();
    }

}
