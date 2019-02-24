struct xttyld {
	struct sgttyb sg;
	struct tchars tc;
	struct queue *qp;
	int status;
	struct block *icanb;
	int col;
};

/* status bits */
#define XTTY_ESCAPED 01
#define XTTY_NEWLINE 02
#define XTTY_STOPPED 04
#define XTTY_FULL 010
