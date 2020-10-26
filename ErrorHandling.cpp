#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
void Error(char c)
{
	int ans = gline;
	//if (s[symcur].isfirst) ans 
	if (c == 'k' && s[symcur-1].isfirst) ans--;

	cout << c << " " << ans << endl;
	fprintf(out, "%c %d\n", c, ans);
}