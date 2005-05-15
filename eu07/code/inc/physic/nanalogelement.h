#ifndef N_ANALOGELEMENT_H
#define N_ANALOGELEMENT_H
//------------------------------------------------------------------------------
/**
    @class nAnalogElement

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#ifndef N_ELEMENT_H
#include "physic/nelement.h"
#endif

#undef N_DEFINES
#define N_DEFINES nAnalogElement
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nAnalogElement : public nElement
{
private:
	double PrevFlow;
public:
    /// constructor
    nAnalogElement();
    /// destructor
    virtual ~nAnalogElement();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void InitQ(double newq);

	virtual void ChangeFlow(double AddFlow);  

	double Press, Flow, Quant;			// cisnienie/napiecie, strumien/prad, ilosc (powietrza, ladunku),
	double Fout;						// pomocnicze: strumien/prad na wyjsciu, modyfikowane przez nastepniki
	bool ShutOff;						//pomocnicze: stan wysokiej impedancji na wyjsciu
	virtual void Update(double dt);
    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

