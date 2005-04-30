source dynamic/PKP/ty2/ty2.tcl

#	visual.addmodel dynamic/pkp/eu07/cabA.ive ""

#nastawnik

	new nsecommands inc_mcp
	inc_mcp.connecttomodelelement visual Nastawnik 0 0.0
	inc_mcp.addcmd1 . mcp 1
#	inc_mcp.addcmd audio/main_ctrl start

	new nsecommands inc_mcp_rep
	inc_mcp_rep.addcmd1 /sys/servers/input getbutton inc_mcp_keyb
	inc_mcp_rep.addcmd inc_mcp execnow
	inc_mcp_rep.addcmd1 inc_mcp_rep exec 0.05


	new nsecommands dec_mcp
	dec_mcp.connecttomodelelement visual Nastawnik 1 0.0
	dec_mcp.addcmd1 . mcp -1
#	dec_mcp.addcmd audio/main_ctrl start

	new nsecommands dec_mcp_rep
	dec_mcp_rep.addcmd1 /sys/servers/input getbutton dec_mcp_keyb
	dec_mcp_rep.addcmd dec_mcp execnow
	dec_mcp_rep.addcmd1 dec_mcp_rep exec 0.05

#----------------------------------------------------------

	setHandle ext1


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
		.map keyb0:num8.down		"script: .setdir 1; .status"
		.map keyb0:num5.down		"script: .setdir 0; .status"
		.map keyb0:num2.down		"script: .setdir -1; .status"
		.map keyb0:num9.down		"script: bcp -1; .status"
		.map keyb0:num3.down		"script: bcp 1; .status"
		.map keyb0:num7.down		"script: hcp -1; .status"
		.map keyb0:num1.down		"script: hcp 1; .status"

		.map keyb0:left.down		"script: dec_mcp.exec 0; dec_mcp_rep.exec 0.2; .status"
		.map keyb0:right.down		"script: inc_mcp.exec 0; inc_mcp_rep.exec 0.2; .status"
	    .map keyb0:right.pressed	inc_mcp_keyb
	    .map keyb0:left.pressed		dec_mcp_keyb

		.map keyb0:up.down			"script: scp -1; .status"
		.map keyb0:down.down		"script: scp  1; .status"


		.map keyb0:space.up			"script: /world.releasemomentary"

	.endmap

#sel ..