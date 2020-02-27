/*
 * AcqSys.h
 *
 *  Created on: 27 Feb 2020
 *      Author: pgm
 */

#ifndef ACQPROC_ACQSYS_H_
#define ACQPROC_ACQSYS_H_

#include <vector>
#include <string>

using namespace std;

struct Dev {
	int devnum;
	int fd;
	void* host_buffer;
	void* lbuf;
//	struct XLLC_DEF xllc_def;
};

/* VI : models Vector Input */
struct VI {
	int len(void) const;
	VI& operator+= (const VI& right);
	int AI16;
	int AI32;
	int DI32;
	int SP32;
	VI();
};

/* VO : Modules Vector Output */
struct VO {
	int len(void) const;
	VO& operator+= (const VO& right);
	int AO16;
	int DO32;
	VO();
};


/* IO Base Class */
class IO {
	string name;
	string* _string;
public:
	const VI vi;
	const VO vo;

	IO(string _name, VI _vi, VO _vo);
	virtual ~IO();
	virtual string toString();
};


class ACQ: public IO
/*< models an ACQ2106 box. */
{
	struct Dev dev;

	ACQ(string _name, VI _vi, VO _vo, VI& sys_vi_cursor, VO& sys_vo_cursor);

public:
	const VI vi_cursor;	/* offset for each Input type in Interface vector */
	const VO vo_cursor;	/* offset for each Output type in Interface vector */

	virtual string toString();

	virtual bool newSample(void);
	/*< checks host buffer for new sample, if so copies to lbuf and reports true */
	virtual unsigned tlatch(void);
	/*< returns latest tlatch from lbuf */
	virtual void arm(int nsamples);
	/*< prepare to run a shot nsamples long, arm the UUT. */
friend class HBA;
};


class HBA: public IO
/*< modules a Host Bus Adapter like AFHBA404. */
{
	HBA(int _devnum, vector <ACQ*> _uuts, VI _vi, VO _vo);
public:
	int devnum;
	vector<ACQ*> uuts;
	const VI vi;
	const VO vo;

	static HBA& create(const char* json_def);

	void dump_config();
	/*< output complete configuration with calculated offsets */
	void dump_data(const char* basename);
	/*< output raw data for each ACQ */
};


#endif /* ACQPROC_ACQSYS_H_ */
