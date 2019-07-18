{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 0,
			"revision" : 1,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
<<<<<<< HEAD
		"rect" : [ 118.0, 134.0, 745.0, 527.0 ],
=======
		"rect" : [ 169.0, 210.0, 745.0, 527.0 ],
>>>>>>> parent of b51289d... actually proper sample rate redection now
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"boxes" : [ 			{
				"box" : 				{
<<<<<<< HEAD
					"id" : "obj-11",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 284.0, 244.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 284.0, 293.0, 65.0, 22.0 ],
					"text" : "window $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-5",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 77.0, 285.0, 46.0, 22.0 ],
					"text" : "rate $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 153.0, 346.0, 66.0, 22.0 ],
					"text" : "colloquial~"
				}

			}
, 			{
				"box" : 				{
=======
>>>>>>> parent of b51289d... actually proper sample rate redection now
					"format" : 6,
					"id" : "obj-9",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
<<<<<<< HEAD
					"patching_rect" : [ 68.0, 249.0, 50.0, 22.0 ]
=======
					"patching_rect" : [ 84.0, 243.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-5",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 294.0, 321.0, 43.0, 22.0 ],
					"text" : "cycle~"
>>>>>>> parent of b51289d... actually proper sample rate redection now
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-4",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 153.0, 402.0, 35.0, 22.0 ],
					"text" : "dac~"
				}

			}
, 			{
				"box" : 				{
					"data" : 					{
						"clips" : [ 							{
								"filename" : "Macintosh HD:/Users/instrument/Desktop/180308-000.wav",
								"filekind" : "audiofile",
								"selection" : [ 0.163461538461538, 0.653846153846154 ],
								"loop" : 1,
								"content_state" : 								{
<<<<<<< HEAD
									"originallengthms" : [ 0.0 ],
									"formant" : [ 1.0 ],
									"timestretch" : [ 0 ],
									"originallength" : [ 0.0, "ticks" ],
									"pitchcorrection" : [ 0 ],
									"play" : [ 0 ],
									"basictuning" : [ 440 ],
									"mode" : [ "basic" ],
									"pitchshift" : [ 1.0 ],
									"originaltempo" : [ 120.0 ],
									"quality" : [ "basic" ],
									"speed" : [ 1.0 ],
									"slurtime" : [ 0.0 ],
									"formantcorrection" : [ 0 ],
									"followglobaltempo" : [ 0 ]
=======
									"formant" : [ 1.0 ],
									"pitchshift" : [ 1.0 ],
									"mode" : [ "basic" ],
									"formantcorrection" : [ 0 ],
									"followglobaltempo" : [ 0 ],
									"pitchcorrection" : [ 0 ],
									"originaltempo" : [ 120.0 ],
									"slurtime" : [ 0.0 ],
									"originallengthms" : [ 0.0 ],
									"quality" : [ "basic" ],
									"basictuning" : [ 440 ],
									"timestretch" : [ 0 ],
									"play" : [ 0 ],
									"speed" : [ 1.0 ],
									"originallength" : [ 0.0, "ticks" ]
>>>>>>> parent of b51289d... actually proper sample rate redection now
								}

							}
 ]
					}
,
					"id" : "obj-3",
					"maxclass" : "playlist~",
					"numinlets" : 1,
					"numoutlets" : 5,
					"outlettype" : [ "signal", "signal", "signal", "", "dictionary" ],
					"patching_rect" : [ 139.0, 153.0, 150.0, 30.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 153.0, 316.0, 66.0, 22.0 ],
					"text" : "colloquial~"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 1 ],
					"order" : 0,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 0 ],
					"order" : 1,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-7", 0 ],
					"source" : [ "obj-11", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"order" : 1,
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"order" : 0,
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-5", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-7", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"source" : [ "obj-9", 0 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "180308-000.wav",
				"bootpath" : "~/Desktop",
				"patcherrelativepath" : "../../../../../../../Desktop",
				"type" : "WAVE",
				"implicit" : 1
			}
, 			{
				"name" : "colloquial~.mxo",
				"type" : "iLaX"
			}
 ],
		"autosave" : 0
	}

}
