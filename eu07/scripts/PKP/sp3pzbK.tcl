	.settype "SEM_"

	new nosgmodel model
	model.setmodel PKP/sp3pzbK.ive ""
	.setvisual model


	new nsecommands sp1
      sp1.addcmd2 model setsequencevalue UpperYellow 1	
      sp1.addcmd2 model setsequencemode  Green stop
	sp1.addcmd2 model setsequencevalue White 1

	new nsecommands sp2
      sp2.addcmd2 model setsequencemode UpperYellow stop	
      sp2.addcmd2 model setsequencevalue Green 1	
	sp2.addcmd2 model setsequencevalue White 1	
	
      new nsecommands sp3
	sp3.addcmd2 model setsequencemode  UpperYellow stop
	sp3.addcmd2 model setsequencemode  Green start
	sp3.addcmd2 model setsequencevalue White 1	

	new nsecommands sp4
	sp4.addcmd2 model setsequencemode  UpperYellow start
	sp4.addcmd2 model setsequencemode  Green stop
	sp4.addcmd2 model setsequencevalue White 1
