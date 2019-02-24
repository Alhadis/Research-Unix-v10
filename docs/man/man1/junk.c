
	class A { public: f(); };
	class B : public virtual A {};
	class C : public A,B { g(); };

	C::g() { f(); }

