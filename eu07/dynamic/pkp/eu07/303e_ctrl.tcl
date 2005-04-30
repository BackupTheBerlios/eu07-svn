source dynamic/PKP/EU07/303e.tcl

	visual.addmodel dynamic/pkp/eu07/cabA.ive ""

	sel audio
		new nsimsound sw
			sel sw
			.setfile "sounds/switch.wav"
			.setambient true
		sel ..

		new nsimsound buzzer
			sel buzzer
			.setfile "sounds/bzyczek.wav"
			.setambient true
			.setlooping true
#			.stop
		sel ..
	sel ..

#----------------------------------------------------------

#nastawnik

addMultiHandle1 mcp "sounds/nastawnik jazdy 16.wav" 0.92 4 "Nastawnik#main_controller" mcp 1

#----------------------------------------------------------

#bocznik

addMultiHandle1 scp "sounds/bocznik 16.wav" 0.92 4 "Bocznik#secondary_controller" scp 1

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
	czuwak_reset.addcmd czuwak_blink_off execnow
	czuwak_reset.addcmd czuwak_buzzer abort
	czuwak_reset.addcmd1 czuwak_blink_on exec 30
	czuwak_reset.addcmd1 czuwak_buzzer exec 35

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


#		.map keyb0:1.down		"script: /world/dynamics/eu07-424.setvel -5.0"
#		.map keyb0:2.down		"script: /world/dynamics/eu07-424.setvel -2.0"
#		.map keyb0:3.down		"script: /world/dynamics/eu07-424.setvel -1.0"
#		.map keyb0:4.down		"script: /world/dynamics/eu07-424.setvel -0.5"
#		.map keyb0:5.down		"script: /world/dynamics/eu07-424.setvel  0.0" 
#		.map keyb0:9.down		"script: /world/dynamics/eu07-424.setvel  5.0"
#		.map keyb0:8.down		"script: /world/dynamics/eu07-424.setvel  2.0"
#		.map keyb0:7.down		"script: /world/dynamics/eu07-424.setvel  1.0"
#		.map keyb0:6.down		"script: /world/dynamics/eu07-424.setvel  0.5"
		.map keyb0:num8.down		"script: inc_dir.execnow; .status"
#		.map keyb0:num5.down		"script: .setdir 0; .status"
		.map keyb0:num2.down		"script: dec_dir.execnow; .status"
		.map keyb0:num9.down		"script: bcp -1; .status"
		.map keyb0:num3.down		"script: bcp 1; .status"
		.map keyb0:num7.down		"script: hcp -1; .status"
		.map keyb0:num1.down		"script: hcp 1; .status"
		.map keyb0:f1.down			"script: setHandle cabA"
		.map keyb0:f2.down			"script: setHandle ext1"

#		.map keyb0:left.down		"script: dec_mcp.exec 0; dec_mcp_rep.exec 0.2; .status"
#		.map keyb0:right.down		"script: inc_mcp.exec 0; inc_mcp_rep.exec 0.2; .status"
#	    .map keyb0:right.pressed	inc_mcp_keyb
#	    .map keyb0:left.pressed		dec_mcp_keyb

		addMultiHandleMappings mcp right left
		addMultiHandleMappings scp down up

		.map keyb0:up.down			"script: scp -1; .status"
		.map keyb0:down.down		"script: scp  1; .status"


		.map keyb0:space.up			"script: /world.releasemomentary"
		.map keyb0:space.down		"script: btn_czuwak.setstate 1;"




	.endmap


#sel ..