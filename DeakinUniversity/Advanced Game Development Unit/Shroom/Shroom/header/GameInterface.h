#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H



// Controllabe objects expose the following interface
template <typename AType>
class IControllable
{

	protected:
		IControllable(){}
		IControllable(IControllable const &){}
		IControllable& operator=(IControllable const &){}

	public:
		typedef AType	ACTION;
		virtual ~IControllable(){}
		virtual bool ProcessAction(const ACTION &a) = 0;	// Process a single action this turn
		virtual bool QueueAction(const ACTION &a) = 0;		// Queue an action
		virtual void ProcessQueue() = 0;					// Process action queue, one per turn until empty
		
};

// Controllers must conform to the following interface
template <typename PType>
class IController
{
	protected:
		IController(){}
		IController(IController const &){}
		IController& operator=(IController const &){}

	public:
		typedef PType	PERCEPT;
		virtual ~IController(){}

		virtual void OnStart() = 0;
		virtual void OnTick(const PERCEPT &p) = 0;
		virtual void OnStop() = 0;
};


#endif