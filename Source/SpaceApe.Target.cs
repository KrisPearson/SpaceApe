// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SpaceApeTarget : TargetRules
{
	public SpaceApeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        bUsesSteam = true;
		ExtraModuleNames.Add("SpaceApe");
	}


    //
    // TargetRules interface.
    //
    /* copied from RnD
    public override void SetupBinaries(
        TargetInfo Target,
        ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
        ref List<string> OutExtraModuleNames
        ) {
        OutExtraModuleNames.Add("RnD");
    }

    */
}
