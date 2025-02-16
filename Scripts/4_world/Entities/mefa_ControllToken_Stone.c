class mefa_ControllToken_Stone : Inventory_Base
{
    private string m_OwnerSteamID; // Uložené SteamID hráče

    void mefa_ControllToken_Stone()
    {
        if (GetGame().IsServer())
        {
            m_OwnerSteamID = ""; // Inicializace proměnné
        }
    }

    // Nastaví SteamID hráče
    void SetOwnerSteamID(string steamID)
    {
        if (GetGame().IsServer())
        {
            m_OwnerSteamID = steamID;
            GetGame().AdminLog("[mefa_ControllToken_Stone] SteamID set: " + steamID);
        }
    }

    // Vrátí SteamID hráče
    string GetOwnerSteamID()
    {
        return m_OwnerSteamID;
    }

    // Uložení dat při restartu serveru (persist)
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_OwnerSteamID);
    }

    // Načtení dat po restartu serveru (persist)
    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version))
            return false;

        string storedSteamID;
        if (ctx.Read(storedSteamID))
        {
            m_OwnerSteamID = storedSteamID;
            return true;
        }
        return false;
    }
};
