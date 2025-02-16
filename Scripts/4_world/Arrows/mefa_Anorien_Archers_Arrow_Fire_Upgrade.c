class mefa_Anorien_Archers_Arrow_Fire_Upgrade : Inventory_Base
{
	MefaTorchLight   m_Light1;
	Particle    m_FireParticle1;
	ref Timer m_DestroyTimer;  // Timer reference
	
	void mefa_Anorien_Archers_Arrow_Fire_Upgrade()
	{
		if (!GetGame().IsMultiplayer() || GetGame().IsClient())
		{
			m_Light1 = MefaTorchLight.Cast(ScriptedLightBase.CreateLight(MefaTorchLight, "0 0 0"));


			m_Light1.AttachOnMemoryPoint(this, "fire1");
			vector selection_pos1 = ModelToWorld(GetMemoryPointPos("fire1"));

			m_FireParticle1 = Particle.PlayOnObject(ParticleList.TORCH_T1, this, selection_pos1, Vector(0, 0, 0), true);
			m_FireParticle1.ScaleParticleParamFromOriginal(EmitorParam.SIZE, 0.4);
			m_FireParticle1.ScaleParticleParamFromOriginal(EmitorParam.VELOCITY, 0.3);
			m_FireParticle1.ScaleParticleParamFromOriginal(EmitorParam.VELOCITY_RND, 0.3);

		}

		// Nastavit ËasovaË pro zniËenÌ svÏtla a zastavenÌ Ë·stice
		m_DestroyTimer = new Timer();
		m_DestroyTimer.Run(10, this, "DestroyLightAndParticle", NULL, false);  // Spusùte jednou po 10 sekund·ch
	}

	void DestroyLightAndParticle()
	{
		// Tato metoda bude vol·na po 30 sekund·ch a zniËÌ svÏtlo a zastavÌ Ë·stici
		if (m_Light1)
		{
			m_Light1.Destroy();
			m_Light1 = NULL;
		}
		if (m_FireParticle1)
		{
			m_FireParticle1.Stop();
			m_FireParticle1 = NULL;
		}
	}

	override void EEInit()
	{
		super.EEInit();

	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		if (m_DestroyTimer)
		{
			m_DestroyTimer.Stop();  // Zastavit ËasovaË p¯i odstranÏnÌ objektu
		}
		DestroyLightAndParticle();  // ZniËenÌ svÏtla a zastavenÌ Ë·stice takÈ p¯i odstranÏnÌ
	}
	
}