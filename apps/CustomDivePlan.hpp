////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <QObject>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "core/sample.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FastDiveInfo
{
public:
	duration_t time;
	duration_t ndl;
	depth_t    depth;

public:
	static FastDiveInfo fromSample (const struct sample & samp) { return FastDiveInfo { .time = samp.time, .ndl = samp.ndl, .depth = samp.depth, }; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CustomDivePlan : public QObject
{
	Q_OBJECT
public:
	void loadLogFiles (const std::string & fname);
	struct diveplan * createDivePlanFromComputer (const struct divecomputer & dc);
	void planDeco (diveplan * divePlan, struct dive * dive);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
