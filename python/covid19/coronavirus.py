#!/usr/bin/env python3


from urllib.error import HTTPError
from datetime import datetime as dt
import os
import pandas as pd
import matplotlib.pyplot as plt
# import matplotlib.dates as mdates
import numpy as np


REGIONS_DIR:str = os.path.join(os.path.dirname(os.path.realpath(__file__)), "regions")

def import_region(name:str):
	subregions:list = open(os.path.join(REGIONS_DIR, name)).read().split("\n")
	
	colour:str = None
	if subregions[0][0] == "#":
		colour = subregions[0][1:]
		subregions = subregions[1:]
	
	return (colour, subregions)

all_regions:dict = {name : import_region(name) for name in os.listdir(REGIONS_DIR)}


def get_population(region:str):
	# Already counted
	if region in ["Hong Kong","Taiwan","Macau","North Ireland"]:
		return 0
	elif region == "Others":
		return 3700
	
	region:dict = {
		"US": "United States",
		"UK": "United Kingdom",
		"Egypt": "Egypt, Arab Rep.",
		"Iran": "Iran, Islamic Rep.",
		"Mainland China": "China",
		"North Macedonia": "Macedonia, FYR",
		"Korea, South": "Korea, Rep.",
		"occupied Palestinian territory": "Palestine",
		"Russia": "Russian Federation",
		"Czechia": "Czech Republic",
	}.get(region) or region
	
	if get_population.df is not None:
		_df = get_population.df.loc[get_population.df["Country Name"]==region]
		try:
			return int(_df.loc[_df["Year"]==_df["Year"].max()].iat[0, 4])
		except IndexError:
			print(_df)
			print(f"No population data for: {region}")
			return 0
	try:
		get_population.df = pd.read_csv("populations.csv")
	except FileNotFoundError:
		get_population.df = pd.read_csv("https://pkgstore.datahub.io/core/population/population_csv/data/ead5be05591360d33ad1a37382f8f8b1/population_csv.csv")
		get_population.df.to_csv("populations.csv")
	return get_population(region)

get_population.df = None

def graph_arr(_min:int, ax, arr, colour:str, name:str, per_capita:bool, country2value:dict, df_mutable, region_list:list, line_width:int, linestyle:str):
	n_infected_now:int = arr[-1]
	if n_infected_now < _min:
		return
	arr.index = pd.to_datetime(arr.index)
	if per_capita or country2value:
		n:int = 0
		for _subregion in region_list:
			if (country2value is not None) and (_subregion not in country2value):
				continue
			try:
				n += get_population(_subregion)
			except TypeError:
				# returned None
				raise Exception(f"Cannot get population for: {_subregion}")
		arr = arr.divide(n)
	arr.plot(ax=ax, linewidth=line_width, linestyle=linestyle, label=name, color=colour)
	plt.annotate(n_infected_now, xy=(plt.gca().get_xlim()[1],n_infected_now))


def graph(name:str, df, df2, yscale:str, per_capita:bool, country2value:dict, countries:list, regions:list, highlight:list, _min:int, split_regions:bool, daily_change:bool, is_total:bool, mark_dates:list):
	drop_cols:list = ["Province/State","Country/Region","Lat","Long"]
	fig, ax = plt.subplots()
	plt.margins(x=0,y=0)
	
	_all_regions:dict = None
	if is_total:
		total_region = set([c for region in regions for c in all_regions[region][1]] + countries)
		_all_regions = {"Total": ("black", total_region)}
		regions = ["Total"]
		countries = []
	else:
		_all_regions = all_regions
	
	df_mutable = df.copy()
	
	for region in regions:
		_colour, _countries = _all_regions[region]
		if split_regions:
			countries += _countries
			continue
		_df = df.loc[df["Country/Region"].isin(_countries)].drop(columns=drop_cols)
		if df2 is not None:
			_df2 = df2.loc[df2["Country/Region"].isin(_countries)].drop(columns=drop_cols)
			_df = _df.div(_df2)
		if daily_change:
			_df = to_daily_change(_df)
		linestyle = "-" if region in highlight else "--"
		graph_arr(_min, ax, _df.sum(axis=0), _colour, region, per_capita, country2value, df_mutable, _countries, 2, linestyle)
	
	for country in countries:
		_df = df.loc[df["Country/Region"]==country].drop(columns=drop_cols)
		if df2 is not None:
			_df2 = df2.loc[df["Country/Region"]==country].drop(columns=drop_cols)
			_df = _df.div(_df2)
		if daily_change:
			_df = to_daily_change(_df)
		linestyle = "-" if country in highlight else "--"
		graph_arr(_min, ax, _df.sum(axis=0), None, country, per_capita, country2value, df_mutable, [country], 1, linestyle)
	
	for date in mark_dates:
		label:str = date
		if "=" in date:
			date, label = date.split("=")
		t:float = dt.strptime(date, "%Y-%m-%d")
		ax.axvline(t, label=label)
	
	if country2value is not None:
		_df = df.set_index("Country/Region").groupby(country2value).drop(columns=drop_cols).sum(axis=0)
	
	#_df.plot(ax=ax)
	
	plt.title(name + (" per day" if daily_change else ""))
	ax.legend(loc="best")
	ax.set_yscale(yscale)

	plt.show()


def get_tbl__base(which:str):
	filename:str = f"time_series_covid19_{which}_global.csv"
	if os.path.isfile(filename) and os.path.getmtime(filename) > (dt.now().timestamp() - 3600*24):
		return pd.read_csv(filename)
	url:str = f"https://raw.githubusercontent.com/CSSEGISandData/COVID-19/master/csse_covid_19_data/csse_covid_19_time_series/{filename}"
	try:
		print("Downloading latest dataset")
		df = pd.read_csv(url)
		df.to_csv(filename)
		return df
	except HTTPError as e:
		raise Exception(f"{e}: {url}")


def is_datetimestr_greater_than(a:str, t:float, default:bool):
	try:
		print(dt.strptime(a, "%m/%d/%y").timestamp(), t)
		return dt.strptime(a, "%m/%d/%y").timestamp() >= t
	except ValueError:
		return default


def get_tbl(which:str, _from:str, _to:str):
	df = get_tbl__base(which)
	
	cols_to_keep:list = df.columns
	if _from is not None:
		cols_to_keep = [c for c in cols_to_keep if is_datetimestr_greater_than(c, dt.strptime(_from, "%Y-%m-%d").timestamp(), True)]
	if _to is not None:
		cols_to_keep = [c for c in cols_to_keep if not is_datetimestr_greater_than(c, dt.strptime(_to,   "%Y-%m-%d").timestamp(), False)]
	
	if _from or _to:
		df = df[cols_to_keep]
	
	print(_from, _to)
	print(df)
	
	return df


def to_daily_change(df):
	df2 = df.drop(columns=df.columns[-1])
	df2.columns = df.columns[1:]
	df = df.drop(columns=df.columns[1])
	df.columns = df2.columns
	return df.subtract(df2)


if __name__ == "__main__":
	import argparse
	
	parser = argparse.ArgumentParser()
	parser.add_argument("which", nargs="*", help="confirmed deaths or recovered")
	parser.add_argument("-a", help="YYYY-MM-DD")
	parser.add_argument("-b", help="YYYY-MM-DD")
	parser.add_argument("--mark-date", action="append", default=[], help="Mark dates on the graph")
	parser.add_argument("--scale", default="log", help="log or linear or symlog or logit")
	parser.add_argument("-r","--regions", nargs="*", help="Regions to graph. If none specified, all included. Options: '"+"' '".join(list(all_regions))+"'")
	parser.add_argument("-R","--Regions", default=[], nargs="+", help="As --regions but highlighted")
	parser.add_argument("-c","--countries", nargs="*", help="As --countries but highlighted")
	parser.add_argument("-C","--Countries", default=[], nargs="*", help="Countries to graph. If none specified, all included.")
	parser.add_argument("--split-regions", default=False, action="store_true", help="Map each country individually in each region")
	parser.add_argument("--min", default=0, type=int, help="Ignore countries with fewer cases")
	parser.add_argument("--per-capita", default=False, action="store_true")
	parser.add_argument("-v","--verbose", default=0, action="count")
	parser.add_argument("-q","--quiet",   default=0, action="count")
	parser.add_argument("-d","--daily-change", default=False, action="store_true")
	parser.add_argument("--total", default=False, action="store_true")
	parser.add_argument("--region-dir", default=False, action="store_true", help="Display the directory where regions are stored, and exit")
	parser.add_argument("--country2value", help="Path of DSV file mapping country name to variable value. If supplied, will display a graph correlating SARS2 data with these values, per capita.")
	args = parser.parse_args()
	
	if args.region_dir:
		print(REGIONS_DIR)
		exit(0)
	
	country2value:dict = None
	if args.country2value is not None:
		import re
		country2value = {x:y for x,y in re.findall("([^\t]+)\t([^\n]+)", open(args.country2value).read())}
	
	verbosity:int = 1 + args.verbose - args.quiet
	
	args.countries = [] if args.countries is None else ["Mainland China","US","UK"] if len(args.countries)==0 else args.countries
	args.regions = [] if args.regions is None else list(all_regions) if len(args.regions)==0 else args.regions
	
	_printed_debug_reports:bool = False
	
	if verbosity > 2:
		print(f"REGIONS_DIR = {REGIONS_DIR}")
	
	for _which in args.which:
		which2:str = None
		if "/" in _which:
			which, which2 = _which.split("/")
		else:
			which = _which
		
		df  = get_tbl(which,  args.a, args.b)
		df2 = get_tbl(which2, args.a, args.b) if which2 is not None else None
		
		if not _printed_debug_reports:
			if verbosity > 0:
				unregioned_countries:list = df.loc[~df["Country/Region"].isin([c for _, r in all_regions.values() for c in r])]["Country/Region"].tolist()
				if len(unregioned_countries) != 0:
					_ = "\n\t"
					print(f"Unregioned countries{_}{_.join(unregioned_countries)}")
			if verbosity > 1:
				for region, (_, countries) in all_regions.items():
					print(region)
					for country in countries:
						print(f"\t{country}")
			_printed_debug_reports = True
		
		graph(_which, df, df2, args.scale, args.per_capita, country2value, args.countries+args.Countries, args.regions+args.Regions, args.Countries+args.Regions, args.min, args.split_regions, args.daily_change, args.total, args.mark_date)
