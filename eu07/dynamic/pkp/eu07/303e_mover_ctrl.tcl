source dynamic/PKP/EU07/303e_mover.tcl
source dynamic/PKP/EU07/303e_cabsnds.tcl


	visual.addmodel dynamic/pkp/eu07/cabA.ive ""

	setvisual visual


#----------------------------------------------------------

#nastawnik

addMultiHandle mcp "sounds/nastawnik jazdy 16.wav" 0.92 4 "Nastawnik#main_controller" ctrl 0 1

#----------------------------------------------------------

#bocznik

addMultiHandle scp "sounds/bocznik 16.wav" 0.92 4 "Bocznik#secondary_controller" ctrl 1 1

#----------------------------------------------------------

#shp

	new nseswitch shp_indicator
	shp_indicator.setelements visual Shp_on Shp_off

#	new nseswitch shp_off
#	shp_off.setelements visual Shp_off Shp_on
#	shp_off.execnow

#----------------------------------------------------------

#czuwak

	new nseswitch czuwak_indicator
	czuwak_indicator.setelements visual Czuwak_on Czuwak_off
#	new nseswitch czuwak_on
#	czuwak_on.setelements visual Czuwak_on Czuwak_off

#	new nseswitch czuwak_off
#	czuwak_off.setelements visual Czuwak_off Czuwak_on
#	czuwak_off.execnow

	new nsecommands czuwak_blink_on
#	czuwak_blink_on.addcmd czuwak_on execnow
	czuwak_blink_on.addcmd czuwak_indicator turnon

	new nsecommands czuwak_blink_off
	czuwak_blink_off.addcmd czuwak_indicator turnoff
#	czuwak_blink_off.addcmd czuwak_off execnow

	czuwak_blink_on.addcmd1 czuwak_blink_off exec 0.5
	czuwak_blink_off.addcmd1 czuwak_blink_on exec 0.5

	new nsecommands czuwak_buzzer
	czuwak_buzzer.addcmd audio/buzzer start


	new nsecommands czuwak_reset
	czuwak_reset.addcmd audio/buzzer stop
	czuwak_reset.addcmd czuwak_blink_on abort
	czuwak_reset.addcmd czuwak_blink_off abort
	czuwak_reset.addcmd czuwak_indicator turnoff
	czuwak_reset.addcmd czuwak_buzzer abort
#	czuwak_reset.addcmd . ismoving
#	czuwak_reset.addcmd1 czuwak_blink_on exec 30
#	czuwak_reset.addcmd1 czuwak_buzzer exec 35

#----------------------------------------------------------

#zdazenia odpalane przy ruszaniu/zatrzymywaniu

	new nsecommands hasler_on
	hasler_on.addcmd audio/hasler start

	new nsecommands hasler_off
	hasler_off.addcmd audio/hasler stop

	hasler_off.addcmd hasler_on abort
	hasler_on.addcmd hasler_off abort

	new nsecommands on_go
	on_go.addcmd1 hasler_on exec 0.5
	on_go.addcmd czuwak_reset execnow

	new nsecommands on_stop
	on_stop.addcmd1 hasler_off exec 0.5
	on_stop.addcmd czuwak_reset execnow

	.setgostopevents on_go on_stop

#----------------------------------------------------------

	add2StateButton styczniki "Przycisk-styczniki" -0.005 0 true
	release_styczniki.addcmd . fuse

	add2StateButton czuwak "Przycisk-czuwak" -0.005 0 true
	release_czuwak.addcmd czuwak_reset execnow
	press_czuwak.addcmd1 czuwak_blink_on exec 2
	press_czuwak.addcmd1 czuwak_buzzer exec 7

	add2StateButton pant1 "SW-pantograf-przod" 0 -45 false
	press_pant1.addcmd2 . setpantographstate 1 up
	release_pant1.addcmd2 . setpantographstate 1 down

	add2StateButton pant2 "SW-pantograf-tyl" 0 -45 false
	press_pant2.addcmd2 . setpantographstate 2 up
	release_pant2.addcmd2 . setpantographstate 2 down

	add2StateButton przetwornica "SW-przetwornica" 0 -45 false
	press_przetwornica.addcmd audio/converter start
	release_przetwornica.addcmd audio/converter stop
	


#----------------------------------------------------------

#nawrotnik

	new nsecommands inc_dir
	inc_dir.connecttomodelelement visual "kluczpodst#dir_key" 0 0.0
	inc_dir.addcmd1 . changedir 1
	inc_dir.addcmd audio/sw start
#	inc_dir.addcmd2 visual/anim setdval "dir_key" -0.5


	new nsecommands dec_dir
	dec_dir.connecttomodelelement visual "kluczpodst#dir_key" 1 0.0
	dec_dir.addcmd1 . changedir -1
	dec_dir.addcmd audio/sw start
#	dec_dir.addcmd2 visual/anim setdval "dir_key" 0.5

#----------------------------------------------------------

	setHandle cabA

	#SW-przetwornica


sel /sys/servers/input
	.beginmap
	    .map relmouse0:btn0.down		"script: /world/camera.pick 0"
	    .map relmouse0:btn1.down		"script: /world/camera.pick 1"
#	    .map relmouse0:btn2.down		"script: /world/camera.pick 2"

	    .map relmouse0:btn0.up			"script: /world.releasemomentary"
	    .map relmouse0:btn1.up			"script: /world.releasemomentary"
#	    .map relmouse0:btn2.up			"script: /world.releasemomentary"

	    .map relmouse0:btn0.pressed		cam_dummy1
	    .map relmouse0:btn1.pressed		cam_dummy2
		.map relmouse0:btn2.pressed		cam_rot

		.map relmouse0:-x			cam_rot_left
		.map relmouse0:+x			cam_rot_right
		.map relmouse0:-y			cam_rot_up
		.map relmouse0:+y			cam_rot_down
		.map relmouse0:-z			cam_ortho_zoom_down
		.map relmouse0:+z			cam_ortho_zoom_up



		.map keyb0:m.down		"script: .main true; .status"

		.map keyb0:num8.down		"script: inc_dir.execnow; .status"
#		.map keyb0:num5.down		"script: .setdir 0; .status"
		.map keyb0:num2.down		"script: dec_dir.execnow; .status"
		.map keyb0:num9.down		"script: ctrl 2 -1; .status"
		.map keyb0:num3.down		"script: ctrl 2 1; .status"
		.map keyb0:num7.down		"script: ctrl 3 -1; .status"
		.map keyb0:num1.down		"script: ctrl 3 1; .status"
		.map keyb0:f1.down			"script: setHandle cabA"
		.map keyb0:f2.down			"script: setHandle ext1"

		addMultiHandleMappings mcp right left
		addMultiHandleMappings scp down up

		.map keyb0:space.up			"script: /world.releasemomentary"
		.map keyb0:space.down		"script: push_czuwak.execnow;"


		.map keyb0:end.down			"script: /sys/servers/gfx.setfov 10"
		.map keyb0:end.up			"script: /sys/servers/gfx.setfov 45"


	.endmap


#sel ..