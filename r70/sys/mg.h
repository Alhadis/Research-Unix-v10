struct mg {
	char busy;
	struct device *addr;
	struct queue *rq;
	struct queue *wq;
};
