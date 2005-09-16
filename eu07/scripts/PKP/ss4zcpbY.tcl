	.settype "SEM_"

	new nosgmodel model
	model.setmodel PKP/SS4zcpbY.ive ""
	.setvisual model


	new nsecommands s1
	s1.addcmd2 model setsequencemode  Green stop
	s1.addcmd2 model setsequencevalue Red 1
	s1.addcmd2 model setsequencemode  LowerYellow stop
	s1.addcmd2 model setsequencemode  White stop
	s1.execnow

	new nsecommands s2
	s2.addcmd2 model setsequencevalue  Green 1
	s2.addcmd2 model setsequencemode  Red stop
	s2.addcmd2 model setsequencemode  LowerYellow stop
	s2.addcmd2 model setsequencemode  White stop
			
	new nsecommands s3
	s3.addcmd2 model setsequencemode  Green start
	s3.addcmd2 model setsequencemode  Red stop
	s3.addcmd2 model setsequencemode  LowerYellow stop
	s3.addcmd2 model setsequencemode  White stop

	new nsecommands s4
	s4.addcmd2 model setsequencemode  Green stop
	s4.addcmd2 model setsequencemode  Red stop
	s4.addcmd2 model setsequencemode  LowerYellow start
	s4.addcmd2 model setsequencemode  White stop

	new nsecommands s5
	s5.addcmd2 model setsequencemode  Green stop
	s5.addcmd2 model setsequencemode  Red stop
	s5.addcmd2 model setsequencevalue  LowerYellow 1
	s5.addcmd2 model setsequencemode  White stop

	new nsecommands s10
	s10.addcmd2 model setsequencevalue Green 1
	s10.addcmd2 model setsequencemode  Red stop
	s10.addcmd2 model setsequencevalue LowerYellow 1
	s10.addcmd2 model setsequencemode  White stop

      new nsecommands ms2
	ms2.addcmd2 model setsequencemode  Green stop
	ms2.addcmd2 model setsequencemode  Red stop
	ms2.addcmd2 model setsequencemode  LowerYellow stop
	ms2.addcmd2 model setsequencevalue White 1

	new nsecommands sz1
	sz1.addcmd2 model setsequencemode  Green stop
	sz1.addcmd2 model setsequencevalue Red 1
	sz1.addcmd2 model setsequencemode  LowerYellow stop
	sz1.addcmd2 model setsequencemode  White start
