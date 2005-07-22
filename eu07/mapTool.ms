

-- plane length:94.0171 width:113.96 pos:[5.41311,-9.68661,0] isSelected:on lengthsegs:1 widthsegs:1

rollout mapTool "Map Tool"
(
	spinner spn1Narc "N" range:[0,90,53] scale:1 type:#integer fieldwidth:35 across:4
	spinner spn1Nmin "'" range:[0,59,0] scale:5 type:#integer fieldwidth:35
	spinner spn1Earc "E" range:[0,179,18] scale:1 type:#integer fieldwidth:35 
	spinner spn1Emin "'" range:[0,59,0] scale:5 type:#integer fieldwidth:35
	
	spinner spn2Narc "N" range:[0,90,52] scale:1 type:#integer fieldwidth:35 across:4
	spinner spn2Nmin "'" range:[0,59,0] scale:5 type:#integer fieldwidth:35
	spinner spn2Earc "E" range:[0,179,19] scale:1 type:#integer fieldwidth:35
	spinner spn2Emin "'" range:[0,59,0] scale:5 type:#integer fieldwidth:35
	
	mapbutton mbMap "Chose Texture Map" across:2
	label lMap "<<undefined>>"
	
	button bCreate "Create Map" enabled:false
	
	on mbMap picked texmap do
	(
		lMap.caption= filenameFromPath texmap.filename
		bCreate.enabled= true
	)
	
	fn calcX Earc Emin = (  -68000.0*(19.0-(Earc+Emin/60.0)) )
	fn calcY Narc Nmin = ( -111000.0*(53.0-(Narc+Nmin/60.0)) )
	on bCreate pressed do
	(
		x1= calcX spn1Earc.value spn1Emin.value
		x2= calcX spn2Earc.value spn2Emin.value
		y1= calcY spn1Narc.value spn1Nmin.value
		y2= calcY spn2Narc.value spn2Nmin.value
		w= x2-x1
		l= y1-y2
		x= x1*0.5+x2*0.5
		y= y1*0.5+y2*0.5

		map= plane length:l width:w pos:[x,y,0] isSelected:on lengthsegs:1 widthsegs:1
		map.material= standard diffuseMap:mbMap.map showInViewport:on
	)
)


if MainFloater != undefined do
(
	closerolloutfloater MainFloater
)		
MainFloater = newRolloutFloater "Map Tool" 250 600 
addRollout mapTool MainFloater