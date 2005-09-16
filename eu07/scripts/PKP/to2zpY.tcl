	.settype "SEM_"

	new nosgmodel model
	model.setmodel PKP/to2zpY.ive ""
	.setvisual model


	new nsecommands os1
      os1.addcmd2 model setsequencemode  Green stop      
      os1.addcmd2 model setsequencevalue LowerYellow 1	

      new nsecommands os2
      os2.addcmd2 model setsequencevalue Green 1	
      os2.addcmd2 model setsequenceode LowerYellow stop	

      new nsecommands os3
	os3.addcmd2 model setsequencemode  Green start
	os3.addcmd2 model setsequencemode  LowerYellow stop
	
	new nsecommands os4
      os4.addcmd2 model setsequencemode  Green stop
	os4.addcmd2 model setsequencemode  LowerYellow start

