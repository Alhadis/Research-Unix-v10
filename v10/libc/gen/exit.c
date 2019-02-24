extern void _cleanup();

void
exit(status)
{
	_cleanup();
	_exit(status);
}
