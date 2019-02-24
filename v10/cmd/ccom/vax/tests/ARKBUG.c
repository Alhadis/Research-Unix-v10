	int a[7][5];
	int (*ap)[5];
	void f() {
		ap = &a[2];
	}
