proc cs { } {
	/sys/servers/net.connect "127.0.0.1"
}

proc con { $address } {
	/sys/servers/net.connect $address
}

proc switchl { name } {
	/world/tracks/$name/left.execnow
}

proc switchr { name } {
	/world/tracks/$name/right.execnow
}

proc addMover { name script_file track } {
	sel /world/dynamics
	source dynamic/$script_file
	/world/dynamics/$name.settrack /world/tracks/$track 0
}

proc addMoverTo { name script_file track prev } {
	sel /world/dynamics
	source dynamic/$script_file
	/world/dynamics/$name.settrack /world/tracks/$track 0
	/world/dynamics/$name.attach 0 /world/dynamics/$prev 3
}

proc addVehicle { name script_file track } {
	sel /world/dynamics
	source dynamic/$script_file
	/world/dynamics/$name.settrack /world/tracks/$track 0
	/world/dynamics/$name.init 0
}

proc addVehicleTo { name script_file track loco } {
	sel /world/dynamics
	source dynamic/$script_file
	sel /world/dynamics
	/world/dynamics/$name.settrack /world/tracks/$track 0
	/world/dynamics/$name.init 0
	/world/dynamics/$name.addnewvehicle $loco back 1
}

proc setView { vehicle handle } {
	sel /world/dynamics/$vehicle
	/world/camera.sethandle $handle
}

proc setHandle { handle } {
	/world/camera.sethandle $handle
}

proc signal { semaphore signal } {
	/world/signals/$semaphore/$signal.execnow
}

proc addMultiHandle { name sound volume numSamples element command par1 par2 } {


	new nstateholder sound$name

	for {set i 0} {$i<$numSamples} {incr i} {

		sel audio
			new nsimsound $name$i
				sel $name$i
				.setfile $sound
				.setambient true
				.setvolume $volume
			sel ..
		sel ..

		new nsecommands audio$i$name
		audio$i$name.addcmd audio/$name$i start

		sound$name.addstate $i audio$i$name 0
	}

	new nsecommands inc_$name
	inc_$name.connecttomodelelement visual $element 0 0.0
	inc_$name.addcmd2 . $command $par1 $par2
	inc_$name.addcmd2 sound$name changestate 1 true


	new nsecommands inc_rep_$name
	inc_rep_$name.addcmd1 /sys/servers/input getbutton inc_keyb_$name
	inc_rep_$name.addcmd inc_$name execnow
	inc_rep_$name.addcmd1 inc_rep_$name exec 0.05


	new nsecommands dec_$name
	dec_$name.connecttomodelelement visual $element 1 0.0
	dec_$name.addcmd2 . $command $par1 -$par2
	dec_$name.addcmd2 sound$name changestate 1 true

	new nsecommands dec_rep_$name
	dec_rep_$name.addcmd1 /sys/servers/input getbutton dec_keyb_$name
	dec_rep_$name.addcmd dec_$name execnow
	dec_rep_$name.addcmd1 dec_rep_$name exec 0.05

}

proc addMultiHandle1 { name sound volume numSamples element command par1 } {


	new nstateholder sound$name

	for {set i 0} {$i<$numSamples} {incr i} {

		sel audio
			new nsimsound $name$i
				sel $name$i
				.setfile $sound
				.setambient true
				.setvolume $volume
			sel ..
		sel ..

		new nsecommands audio$i$name
		audio$i$name.addcmd audio/$name$i start

		sound$name.addstate $i audio$i$name 0
	}

	new nsecommands inc_$name
	inc_$name.connecttomodelelement visual $element 0 0.0
	inc_$name.addcmd1 . $command $par1
	inc_$name.addcmd2 sound$name changestate 1 true


	new nsecommands inc_rep_$name
	inc_rep_$name.addcmd1 /sys/servers/input getbutton inc_keyb_$name
	inc_rep_$name.addcmd inc_$name execnow
	inc_rep_$name.addcmd1 inc_rep_$name exec 0.05


	new nsecommands dec_$name
	dec_$name.connecttomodelelement visual $element 1 0.0
	dec_$name.addcmd1 . $command -$par1
	dec_$name.addcmd2 sound$name changestate 1 true

	new nsecommands dec_rep_$name
	dec_rep_$name.addcmd1 /sys/servers/input getbutton dec_keyb_$name
	dec_rep_$name.addcmd dec_$name execnow
	dec_rep_$name.addcmd1 dec_rep_$name exec 0.05

}

proc addMultiHandleMappings { name inc_key dec_key } {
		.map keyb0:$dec_key.down		"script: dec_$name.exec 0; dec_rep_$name.exec 0.2; .status"
		.map keyb0:$inc_key.down		"script: inc_$name.exec 0; inc_rep_$name.exec 0.2; .status"
	    .map keyb0:$inc_key.pressed		inc_keyb_$name
	    .map keyb0:$dec_key.pressed		dec_keyb_$name
}

proc add2StateButton { name element dz rot momentary} {

	new nsesetmatrix release_anim_$name
	release_anim_$name.setelement visual $element

	new nsecommands release_$name
	release_$name.addcmd audio/sw start
	release_$name.addcmd release_anim_$name execnow


	new nsesetmatrix press_anim_$name
	press_anim_$name.setelement visual $element
	press_anim_$name.pretranslate 0 0 $dz
	press_anim_$name.prerotate $rot 0 1 0

	new nsecommands press_$name
	press_$name.addcmd audio/sw start
	press_$name.addcmd press_anim_$name execnow



	new nstateholder btn_$name
	btn_$name.setmomentary $momentary
	btn_$name.addstate 0 release_$name 0
	btn_$name.addstate 1 press_$name 0

	new nsecommands push_$name
	push_$name.connecttomodelelement visual $element 0 0.0
	push_$name.addcmd2 btn_$name changestate 1 true

}

proc tmp {} {
	new nsecommands nastawnik1
	nastawnik1.addcmd audio/main_ctrl1 start
	new nsecommands nastawnik2
	nastawnik2.addcmd audio/main_ctrl2 start
	new nsecommands nastawnik3
	nastawnik3.addcmd audio/main_ctrl3 start
	new nsecommands nastawnik4
	nastawnik4.addcmd audio/main_ctrl4 start

	new nstateholder nastawnik_sound
	nastawnik_sound.addstate 0 nastawnik1 0
	nastawnik_sound.addstate 1 nastawnik2 0
	nastawnik_sound.addstate 2 nastawnik3 0
	nastawnik_sound.addstate 3 nastawnik4 0

	new nsecommands inc_mcp
	inc_mcp.connecttomodelelement visual "Nastawnik#main_controller" 0 0.0
#	inc_mcp.addcmd1 . mcp 1
	inc_mcp.addcmd2 . ctrl 0 1
	inc_mcp.addcmd2 nastawnik_sound changestate 1 true
#	inc_mcp.addcmd audio/main_ctrl start

	new nsecommands inc_mcp_rep
	inc_mcp_rep.addcmd1 /sys/servers/input getbutton inc_mcp_keyb
	inc_mcp_rep.addcmd inc_mcp execnow
	inc_mcp_rep.addcmd1 inc_mcp_rep exec 0.05


	new nsecommands dec_mcp
	dec_mcp.connecttomodelelement visual "Nastawnik#main_controller" 1 0.0
#	dec_mcp.addcmd1 . mcp -1
	dec_mcp.addcmd2 . ctrl 0 -1
	 
	dec_mcp.addcmd2 nastawnik_sound changestate 1 true
#	dec_mcp.addcmd audio/main_ctrl start

	new nsecommands dec_mcp_rep
	dec_mcp_rep.addcmd1 /sys/servers/input getbutton dec_mcp_keyb
	dec_mcp_rep.addcmd dec_mcp execnow
	dec_mcp_rep.addcmd1 dec_mcp_rep exec 0.05


		new nsimsound main_ctrl1
			sel main_ctrl1
			.setfile "sounds/nastawnik jazdy 16.wav"
			.setambient true
			.setvlomue 0.92

		sel ..
		new nsimsound main_ctrl2
			sel main_ctrl2
			.setfile "sounds/nastawnik jazdy 16.wav"
			.setambient true
			.setvlomue 0.92

		sel ..
		new nsimsound main_ctrl3
			sel main_ctrl3
			.setfile "sounds/nastawnik jazdy 16.wav"
			.setambient true
			.setvlomue 0.92

		sel ..
		new nsimsound main_ctrl4
			sel main_ctrl4
			.setfile "sounds/nastawnik jazdy 16.wav"
			.setambient true
			.setvlomue 0.92
		sel ..



	new nsesetmatrix btn_czuwak_release_anim
	btn_czuwak_release_anim.setelement visual Przycisk-czuwak

	new nsecommands btn_czuwak_release
	btn_czuwak_release.addcmd audio/sw start
	btn_czuwak_release.addcmd btn_czuwak_release_anim execnow
	btn_czuwak_release.addcmd czuwak_reset execnow

	new nsesetmatrix btn_czuwak_press_anim
	btn_czuwak_press_anim.setelement visual Przycisk-czuwak
	btn_czuwak_press_anim.pretranslate 0 0 -0.005

	new nsecommands btn_czuwak_press
	btn_czuwak_press.addcmd audio/sw start
	btn_czuwak_press.addcmd btn_czuwak_press_anim execnow
	btn_czuwak_press.addcmd1 czuwak_blink_on exec 2
	btn_czuwak_press.addcmd1 czuwak_buzzer exec 7


	new nstateholder btn_czuwak
	btn_czuwak.setmomentary true
	btn_czuwak.addstate 0 btn_czuwak_release 0
	btn_czuwak.addstate 1 btn_czuwak_press 0

	new nsecommands btn_czuwak_push
	btn_czuwak_push.connecttomodelelement visual Przycisk-czuwak 0 0.0
	btn_czuwak_push.addcmd1 btn_czuwak setstate 1
}