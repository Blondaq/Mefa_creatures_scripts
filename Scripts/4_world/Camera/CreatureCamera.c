class CreatureCamera extends Camera {
    float sendUpdateAcc = 0.0;
    Object camTarget;
    vector lastValidPosition = "0 0 0";
    vector lastValidOrientation = "0 0 0";  // Poslední platná orientace kamery
    bool isFreelookActive = false;
    vector freelookOrientation;

    // Maximální vzdálenost cíle
    const float maxTargetDistance = 100.0;

    // Aktuální označený cíl
    Object currentTarget = null;

     // Proměnná pro periodické aktualizace target informací
    float targetUpdateAcc = 0.0;
    const float TARGET_UPDATE_INTERVAL = 0.5; // Aktualizovat každých 0.5 sekundy


    void CreatureCamera() {
        SetEventMask(EntityEvent.FRAME);
        camTarget = NULL;
        freelookOrientation = "0 0 0";
    }

    void SetTarget(Object target) {
        camTarget = target;
        lastValidPosition = target.GetPosition() + "0 3 -5";
        lastValidOrientation = "0 0 0";
    }

    override void EOnFrame(IEntity other, float timeSlice) {
        OnUpdate(timeSlice);

        if (sendUpdateAcc > 0.1) {
            GetGame().UpdateSpectatorPosition(GetPosition());
            sendUpdateAcc = 0;
        }
        sendUpdateAcc += timeSlice;

         // Pokud máme aktivní cíl, periodicky odesílej RPC požadavek pro získání informací o cíli
        if (currentTarget) {
            targetUpdateAcc += timeSlice;
            if (targetUpdateAcc >= TARGET_UPDATE_INTERVAL) {
                RequestTargetInfo(currentTarget);
                targetUpdateAcc = 0;
            }
        }
    }

   void OnUpdate(float timeSlice) {
    if (!camTarget) return;

   if (!camTarget.IsInherited(mefa_Humanoid_Logic_Base) || camTarget.GetType().IndexOf("Controlled") == -1) {
        GetGame().ChatPlayer("Camera target is not a valid Controlled entity.");
        return;
    }
    mefa_Humanoid_Logic_Base targetCreature = mefa_Humanoid_Logic_Base.Cast(camTarget);

    if (!targetCreature) return;

    Input input = GetGame().GetInput();
    vector offset = "0 4.2 -6";
    vector smoothedOrientation;
    float currentYaw;
    float targetYaw;
    float interpolatedYaw;
   

    // Detekce kliknutí levého tlačítka myši
    if (input.LocalPress("UAMefaLeftClick")) // Levé tlačítko myši
    {
        float rayDistance = 1000.0; // Maximální vzdálenost paprsku

        // Získání pozice kamery
        vector rayStart = GetGame().GetCurrentCameraPosition(); // Startovní bod je pozice kamery
        vector screenToWorldDirection = GetGame().GetPointerDirection(); // Převod na směr ve světě (bez parametrů)
        vector rayEnd = rayStart + (screenToWorldDirection * rayDistance); // Konec paprsku

        // Výsledky raycastu
        vector contactPos, contactDir;
        int contactComponent;
        set<Object> hitObjects = new set<Object>(); // Kolekce pro detekované objekty
        // Raycast pro detekci kolizí
       // V části, kde se zpracovává výsledek raycastu:
        // Raycast pro detekci kolizí
        if (DayZPhysics.RaycastRV(rayStart, rayEnd, contactPos, contactDir, contactComponent, hitObjects, null, null, false, false, ObjIntersectIFire))
        {
            if (hitObjects.Count() > 0)
            {
                // Předpokládáme, že první objekt je náš cíl
                Object hitObject = hitObjects[0];
                if (hitObject)
                {
                    vector playerPosition = GetGame().GetPlayer().GetPosition();
                    float distance = vector.Distance(playerPosition, contactPos);

                    if (distance <= rayDistance) 
                    {
                        string className = hitObject.GetType();

                        // Pokud je platný cíl (ZombieBase nebo AnimalBase)
                        if (hitObject.IsInherited(ZombieBase) || hitObject.IsInherited(AnimalBase))
                        {
                            GetGame().ChatPlayer("Target (Zombie/Animal) selected: " + className);
                            GetGame().ChatPlayer("Distance: " + distance.ToString() + " meters.");
                            currentTarget = hitObject; // Nastavíme aktuální cíl
                            RequestTargetInfo(hitObject); // Odeslání RPC s platným cílem
                        }
                        else
                        {
                            GetGame().ChatPlayer("Object selected: " + className + " at distance: " + distance.ToString() + " meters.");
                           
                        }
                    }
                    else 
                    {
                      
                    }
                }
                else
                {
                  
                }
            }
            else 
            {
               
            }
        }

    }
   /* if (input.LocalPress("UAMefaTabHold")) { // Hold Tab
        GetGame().ChatPlayer("NEW UI - holding Tab ");
    }*/
    if (input.LocalPress("UAMefaRightClick")) { // Pravé tlačítko myši
        // Pokud raycast nenašel žádný objekt, smažeme aktuální cíl a odešleme prázdný RPC
                currentTarget = null;
                RequestTargetInfo(null);
                GetGame().ChatPlayer("02 - No valid target detected.");
    }
    if (input.LocalValue("UAMefaMiddleHold") > 0) {
        if (!isFreelookActive) {
            isFreelookActive = true;
            GetGame().GetUIManager().ShowCursor(false);
            freelookOrientation = GetOrientation();
        }

        float pitchDiff = -(input.LocalValue("UAAimUp") - input.LocalValue("UAAimDown"));
        float yawDiff = -(input.LocalValue("UAAimLeft") - input.LocalValue("UAAimRight"));

        freelookOrientation[0] = NormalizeAngle(freelookOrientation[0] + Math.RAD2DEG * yawDiff * timeSlice);
        freelookOrientation[1] = Math.Clamp(freelookOrientation[1] - Math.RAD2DEG * pitchDiff * timeSlice, -60, 60);

        currentYaw = lastValidOrientation[0];
        targetYaw = freelookOrientation[0];
        interpolatedYaw = InterpolateYaw(currentYaw, targetYaw, timeSlice * 5.0);

        smoothedOrientation = Vector(interpolatedYaw, freelookOrientation[1], 0);
        SetOrientation(smoothedOrientation);
        lastValidOrientation = smoothedOrientation;

        vector followPosition = targetCreature.ModelToWorld(offset);
        SetPosition(followPosition);
    }
    else 
    {
        if (isFreelookActive) {
            isFreelookActive = false;
        }
        GetGame().GetUIManager().ShowCursor(true);

        vector desiredPosition = targetCreature.ModelToWorld(offset);
        float terrainHeight = GetGame().SurfaceY(desiredPosition[0], desiredPosition[2]);
        if (desiredPosition[1] < terrainHeight + 0.5) {
            desiredPosition[1] = terrainHeight + 0.5;
        }

        // Zvýšená rychlost interpolace pro pozici (např. 20x místo okamžitého skoku)
        float interpolationSpeed = 20.0; 
        lastValidPosition = ManualLerp(lastValidPosition, desiredPosition, timeSlice * interpolationSpeed);
        SetPosition(lastValidPosition);

        vector targetOrientation = targetCreature.GetOrientation();
        targetOrientation[1] = -12; // pevný vertikální úhel
        targetOrientation[2] = 0;

        // Zvýšená rychlost interpolace pro rotaci
        float interpolationYawSpeed = 20.0; 
        currentYaw = lastValidOrientation[0];
        targetYaw = targetOrientation[0];
        interpolatedYaw = InterpolateYaw(currentYaw, targetYaw, timeSlice * interpolationYawSpeed);
        smoothedOrientation = Vector(interpolatedYaw, targetOrientation[1], 0);
        SetOrientation(smoothedOrientation);
        lastValidOrientation = smoothedOrientation;
    }



    // Debug logy
    vector currentPosition = GetPosition();
    vector targetPosition = targetCreature.ModelToWorld(offset);
    vector currentOrientation = GetOrientation();
    currentYaw = currentOrientation[0];
    targetYaw = lastValidOrientation[0];
    float verticalAngle = currentOrientation[1];

   /* GetGame().ChatPlayer("=== Kamera Debug Log ==="); 
    GetGame().ChatPlayer("Aktuální pozice kamery: " + currentPosition);
    GetGame().ChatPlayer("Cílová pozice: " + targetPosition);
    GetGame().ChatPlayer("Poslední validní pozice: " + lastValidPosition);
    GetGame().ChatPlayer("Orientace kamery: " + currentOrientation);
    GetGame().ChatPlayer("Yaw úhel (aktuální -> cílový): " + currentYaw + " -> " + targetYaw + " | Interpolovaný: " + smoothedOrientation[0]);
    GetGame().ChatPlayer("Vertikální úhel (verticalAngle): " + verticalAngle);
    GetGame().ChatPlayer("=========================");*/
}


void RequestTargetInfo(Object target) {
    if (GetGame().IsClient()) {
        Param1<Object> rpcData = new Param1<Object>(target);
        // Používáme RPC pro TargetInfoTarget (jak jste nastavili dříve)
        GetRPCManager().SendRPC("TargetInfoTarget", "HandleTargetInfoTargetRequest", rpcData, true, null);
    }
}





// Normalizace úhlu na rozsah -180° až 180°
float NormalizeAngle(float angle) {
    while (angle > 180) angle -= 360;
    while (angle < -180) angle += 360;
    return angle;
}

// Výpočet interpolace yaw s přechodem přes ±180°
float InterpolateYaw(float currentYaw, float targetYaw, float t) {
    currentYaw = NormalizeAngle(currentYaw);
    targetYaw = NormalizeAngle(targetYaw);

    // Rozdíl úhlů
    float delta = targetYaw - currentYaw;
    delta = NormalizeAngle(delta); // Normalizace rozdílu na rozsah -180° až 180°

    // Interpolace
    float interpolatedYaw = currentYaw + delta * Math.Clamp(t, 0.0, 1.0);
    return NormalizeAngle(interpolatedYaw); // Normalizace výsledku
}

// Přetížené funkce pro interpolaci
vector ManualLerp(vector from, vector to, float t) {
    return from + (to - from) * Math.Clamp(t, 0.0, 1.0);
}

float ManualLerp(float from, float to, float t) {
    return from + (to - from) * Math.Clamp(t, 0.0, 1.0);
}


}
