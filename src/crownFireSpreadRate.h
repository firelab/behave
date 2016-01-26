#ifndef CROWNFIRESPREADRATE_HEADER
#define CROWNFIRESPREADRATE_HEADER

class CrownFireSpreadRate
{
	
public:
	CrownFireSpreadRate();
	~CrownFireSpreadRate();

	double FBL_CrownFireSpreadRate( double windAt20Ft, double mc1, double mc10,
		double mc100, double mcWood );

private:
	static const int	MAX_CATS = 2;	//!< Life categories
	static const int	DEAD_CAT = 0;	//!< Dead life category index
	static const int	LIVE_CAT = 1;	//!< Live life category index

};

#endif // CROWNFIRESPREADRATE_HEADER