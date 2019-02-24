struct{
	struct{
		struct{
			struct{
				long _selection;
				long _state;
			}_S;
		}_CK;
		struct{
			struct{
				long _selection;
				long _state;
			}_S;
		}_D;
		struct{
			long _q;
			long _qn;
			struct{
				long _q;
				long _qn;
				struct{
					long _selection;
					struct{
						struct{
							struct{
								long _selection;
								long _state;
							}_OM;
						}_G;
					}_GT[2];
				}_MX;
				struct{
					struct{
						struct{
							struct{
								long _selection;
								long _state;
							}_OM;
						}_G;
					}_IG;
				}_IN[2];
			}_R[2];
		}_FF;
		struct{
			long _q;
			long _qn;
			struct{
				long _state;
			}_B[2];
		}_ff;
		struct{
			long _state;
		}_TASK;
		struct{
			long _selection;
			long _state;
		}_INIT;
		struct{
			struct{
				long _state;
			}_K;
			struct{
				long _state;
			}_STEP;
		}_SETUP;
		struct{
			long _selection;
			struct{
				long _selection;
				long _state;
			}_G[2];
			struct{
				struct{
					long _state;
				}_K;
				struct{
					long _state;
				}_STEP;
			}_R;
		}_PROP;
		struct{
			long _selection;
			struct{
				long _selection;
				long _state;
			}_G[2];
		}_MINT;
	}_TEST;
}_;

crank(){

						struct local{
							int a,b,e,g,j,s,n,p,r,l,L;
							long _state[3];
							char q[3];
						};
						register struct local *S;
S->_state[S->n]=((_._TEST._CK._S._selection!=1)?0:(_._TEST._D._S._selection!=mod(_._TEST._SETUP._STEP._state,2))?_._TEST._D._S._selection:0?_._TEST._FF._R[0]._IN[0]._IG._G._OM._selection:(((_._TEST._SETUP._STEP._state<=1)&&(_._TEST._FF._R[0]._MX._GT[0]._G._OM._selection==mod(_._TEST._SETUP._STEP._state,2))&&(_._TEST._FF._R[0]._MX._GT[0]._G._OM._selection==_._TEST._FF._R[0]._MX._GT[0]._G._OM._state))||((2<=_._TEST._SETUP._STEP._state)&&(_._TEST._SETUP._STEP._state<=3)&&(_._TEST._FF._R[0]._IN[0]._IG._G._OM._selection==mod((_._TEST._SETUP._STEP._state+1),2))&&(_._TEST._FF._R[0]._IN[0]._IG._G._OM._selection==_._TEST._FF._R[0]._IN[0]._IG._G._OM._state))||((4<=_._TEST._SETUP._STEP._state)&&(_._TEST._SETUP._STEP._state<=5)&&(_._TEST._FF._R[0]._IN[1]._IG._G._OM._selection==mod(_._TEST._SETUP._STEP._state,2))&&(_._TEST._FF._R[0]._IN[1]._IG._G._OM._selection==_._TEST._FF._R[0]._IN[1]._IG._G._OM._state)))?(_._TEST._SETUP._STEP._state+2):_._TEST._SETUP._STEP._state);
}
