	.settype "SEM_"

	new nosgmodel model
	model.setmodel PKP/SS5zpcpbZY.ive ""
	.setvisual model


	new nsecommands s1
	s1.addcmd2 model setsequencemode  Green stop
	s1.addcmd2 model setsequencemode  UpperYellow stop
	s1.addcmd2 model setsequencemode  Red stop
	s1.addcmd2 model setsequencevalue Red 1
	s1.addcmd2 model setsequencemode  LowerYellow stop
	s1.addcmd2 model setsequencemode  White stop
	s1.addcmd2 model setsequencemode  GreenLine stop
      s1.execnow

	new nsecommands s2
	s2.addcmd2 model setsequencevalue Green 1
	s2.addcmd2 model setsequencemode  UpperYellow stop
	s2.addcmd2 model setsequencemode  Red stop
	s2.addcmd2 model setsequencemode  LowerYellow stop
	s2.addcmd2 model setsequencemode  White stop
      s2.addcmd2 model setsequencemode  GreenLine stop
	
	new nsecommands s3
	s3.addcmd2 model setsequencemode  Green start
	s3.addcmd2 model setsequencemode  UpperYellow stop
	s3.addcmd2 model setsequencemode  Red stop
	s3.addcmd2 model setsequencemode  LowerYellow stop
	s3.addcmd2 model setsequencemode  White stop
      s3.addcmd2 model setsequencemode  GreenLine stop
	
      new nsecommands s4
	s4.addcmd2 model setsequencemode  Green stop
	s4.addcmd2 model setsequencemode  UpperYellow start
	s4.addcmd2 model setsequencemode  Red stop
	s4.addcmd2 model setsequencemode  LowerYellow stop
	s4.addcmd2 model setsequencemode  White stop
      s4.addcmd2 model setsequencemode  GreenLine stop

	new nsecommands s5
	s5.addcmd2 model setsequencemode  Green stop
	s5.addcmd2 model setsequencevalue UpperYellow 1
	s5.addcmd2 model setsequencemode  Red stop
	s5.addcmd2 model setsequencemode  LowerYellow stop
	s5.addcmd2 model setsequencemode  White stop
	s5.addcmd2 model setsequencemode  GreenLine stop
	
      new nsecommands s6
	s6.addcmd2 model setsequencevalue  Green 1
	s6.addcmd2 model setsequencemode UpperYellow stop
	s6.addcmd2 model setsequencemode  Red stop
	s6.addcmd2 model setsequencevalue  LowerYellow 1
	s6.addcmd2 model setsequencemode  White stop
	s6.addcmd2 model setsequencevalue  GreenLine 1
	
      new nsecommands s7
	s7.addcmd2 model setsequencemode  Green start
	s7.addcmd2 model setsequencemode UpperYellow stop
	s7.addcmd2 model setsequencemode  Red stop
	s7.addcmd2 model setsequencevalue  LowerYellow 1
	s7.addcmd2 model setsequencemode  White stop
	s7.addcmd2 model setsequencevalue  GreenLine 1

      new nsecommands s8
	s8.addcmd2 model setsequencemode  Green stop
	s8.addcmd2 model setsequencemode UpperYellow start
	s8.addcmd2 model setsequencemode  Red stop
	s8.addcmd2 model setsequencevalue  LowerYellow 1
	s8.addcmd2 model setsequencemode  White stop
	s8.addcmd2 model setsequencevalue  GreenLine 1

      new nsecommands s9
	s9.addcmd2 model setsequencemode  Green stop
	s9.addcmd2 model setsequencevalue UpperYellow 1
	s9.addcmd2 model setsequencemode  Red stop
	s9.addcmd2 model setsequencevalue  LowerYellow 1
	s9.addcmd2 model setsequencemode  White stop
	s9.addcmd2 model setsequencevalue  GreenLine 1

      new nsecommands s10
	s10.addcmd2 model setsequencevalue Green 1
	s10.addcmd2 model setsequencemode  UpperYellow stop
	s10.addcmd2 model setsequencemode  Red stop
	s10.addcmd2 model setsequencevalue  LowerYellow 1
	s10.addcmd2 model setsequencemode  White stop
      s10.addcmd2 model setsequencemode  GreenLine stop

	new nsecommands s11
	s11.addcmd2 model setsequencemode  Green start
	s11.addcmd2 model setsequencemode  UpperYellow stop
	s11.addcmd2 model setsequencemode  Red stop
	s11.addcmd2 model setsequencevalue LowerYellow 1
	s11.addcmd2 model setsequencemode  White stop
      s11.addcmd2 model setsequencemode  GreenLine stop

	new nsecommands s12
	s12.addcmd2 model setsequencemode  Green stop
	s12.addcmd2 model setsequencemode  UpperYellow start
	s12.addcmd2 model setsequencemode  Red stop
	s12.addcmd2 model setsequencevalue LowerYellow 1
	s12.addcmd2 model setsequencemode  White stop
      s12.addcmd2 model setsequencemode  GreenLine stop

	new nsecommands s13
	s13.addcmd2 model setsequencemode  Green stop
	s13.addcmd2 model setsequencevalue UpperYellow 1
	s13.addcmd2 model setsequencemode  Red stop
	s13.addcmd2 model setsequencevalue LowerYellow 1
	s13.addcmd2 model setsequencemode  White stop
      s13.addcmd2 model setsequencemode  GreenLine stop
	
      new nsecommands ms2
	ms2.addcmd2 model setsequencemode  Green stop
	ms2.addcmd2 model setsequencemode  UpperYellow stop
	ms2.addcmd2 model setsequencemode  Red stop
	ms2.addcmd2 model setsequencemode  LowerYellow stop
	ms2.addcmd2 model setsequencevalue White 1
      ms2.addcmd2 model setsequencemode  GreenLine stop

	new nsecommands sz1
	sz1.addcmd2 model setsequencemode  Green stop
	sz1.addcmd2 model setsequencemode  UpperYellow stop
	sz1.addcmd2 model setsequencevalue Red 1
	sz1.addcmd2 model setsequencemode  LowerYellow stop
	sz1.addcmd2 model setsequencemode  White start
      sz1.addcmd2 model setsequencemode  GreenLine stop