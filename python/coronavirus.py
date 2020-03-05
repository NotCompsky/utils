#!/usr/bin/env python3


european_countries:list = [
	"Switzerland",
	"Greece",
	"Spain",
	"North Macedonia",
	"UK",
	"Sweden",
	"Denmark",
	"Estonia",
	"Netherlands", 
	"France",
	"Russia",
	"Croatia",
	"Italy",
	"Romania",
	"Austria",
	"Belgium",
	"Germany",
	"Georgia",
	"Norway",
	"Finland",
	"San Marino", # ???
	"North Ireland",
	"Lithuania",
	"Belarus",
	"Iceland",
	"Ireland",
	"Luxembourg",
	"Monaco",
	"Czech Republic",
	"Armenia",
	"Portugal",
	"Latvia",
	"Ukraine",
]
asia_excl_china:list = ['Philippines', 'Vietnam', 'India', 'Pakistan', 'Nepal', 'Malaysia', 'Japan', 'Sri Lanka', 'Hong Kong', 'Taiwan', 'Macau', 'Cambodia', 'Singapore', 'Thailand', 'South Korea',"Indonesia"]
semitia:list = ["Egypt","Algeria","Iran","Iraq","Afghanistan","Azerbaijan","Lebanon","Israel","Kuwait","Oman","United Arab Emirates","Bahrain","Qatar","Saudi Arabia","Jordan"]
canadia:list = ["Canada","US","Mexico"]
auzealand:list = ["Australia","New Zealand"]
samerica:list = ["Brazil","Ecuador","Argentina","Chile","Morocco"]
china:list = ["Mainland China"]
africa:list = ["Nigeria","Senegal"]
international:list = ["Others"]

all_regions:dict = {
	"Australasia": auzealand,
	"China": china,
	"East asia excluding China": asia_excl_china,
	"Europe": european_countries,
	"Middle East": semitia,
	"North America": canadia,
	"South America": samerica,
	"Sub-Saharan Africa": africa,
	"International": international,
}


def get_population(region:str):
	# Already counted
	if region in ["Hong Kong","Taiwan","Macau","North Ireland"]:
		return 0
	elif region == "Others":
		return 3700
	
	region = {
		"US": "United States",
		"UK": "United Kingdom",
		"Egypt": "Egypt, Arab Rep.",
		"Iran": "Iran, Islamic Rep.",
		"Mainland China": "China",
		"North Macedonia": "Macedonia, FYR",
		"Russia": "Russian Federation",
		"South Korea": "Korea, Rep.",
	}.get(region) or region
	
	if get_population.df is not None:
		_df = get_population.df.loc[get_population.df["Country Name"]==region]
		try:
			return int(_df.loc[_df["Year"]==_df["Year"].max()].iat[0, 4])
		except IndexError:
			print(_df)
			raise Exception(f"No population data for: {region}")
	try:
		get_population.df = pd.read_csv("populations.csv")
	except FileNotFoundError:
		get_population.df = pd.read_csv("https://pkgstore.datahub.io/core/population/population_csv/data/ead5be05591360d33ad1a37382f8f8b1/population_csv.csv")
		get_population.df.to_csv("populations.csv")
	return get_population(region)

get_population.df = None

def graph_arr(_min:int, ax, arr, name:str, per_capita:bool, region_list:list, line_width:int):
	n_infected_now:int = arr[-1]
	if n_infected_now < _min:
		return
	arr.index = pd.to_datetime(arr.index)
	if per_capita:
		n:int = 0
		for _subregion in region_list:
			try:
				n += get_population(_subregion)
			except TypeError:
				# returned None
				raise Exception(f"Cannot get population for: {_subregion}")
		arr = arr.divide(n)
	arr.plot(ax=ax, linewidth=line_width, label=name)
	plt.annotate(n_infected_now, xy=(plt.gca().get_xlim()[1],n_infected_now))


def graph(df, yscale:str, per_capita:bool, countries:list, regions:list, _min:int, split_regions:bool):
	drop_cols:list = ["Province/State","Country/Region","Lat","Long"]
	fig, ax = plt.subplots()
	plt.margins(x=0,y=0)
	
	for region in regions:
		_countries = all_regions[region]
		if split_regions:
			countries += _countries
			continue
		_df = df.loc[df["Country/Region"].isin(_countries)].drop(columns=drop_cols)
		graph_arr(_min, ax, _df.sum(axis=0), region, per_capita, _countries, 2)
	
	for country in countries:
		_df = df.loc[df["Country/Region"]==country].drop(columns=drop_cols)
		graph_arr(_min, ax, _df.sum(axis=0), country, per_capita, [country], 1)
	
	ax.legend(loc="best")
	ax.set_yscale(yscale)

	plt.show()


if __name__ == "__main__":
	import argparse
	
	parser = argparse.ArgumentParser()
	parser.add_argument("which", nargs="*", help="Confirmed Deaths or Recovered")
	parser.add_argument("--scale", default="log", help="log or linear or symlog or logit")
	parser.add_argument("-r","--regions", nargs="*", help="Regions to graph. If none specified, all included. Options: '"+"' '".join(list(all_regions))+"'")
	parser.add_argument("-c","--countries", nargs="*", help="Countries to graph. If none specified, all included.")
	parser.add_argument("--split-regions", default=False, action="store_true", help="Map each country individually in each region")
	parser.add_argument("--min", default=0, type=int, help="Ignore countries with fewer cases")
	parser.add_argument("--per-capita", default=False, action="store_true")
	parser.add_argument("-v","--verbose", default=0, action="count")
	args = parser.parse_args()
	
	args.countries = [] if args.countries is None else ["Mainland China","US","UK"] if len(args.countries)==0 else args.countries
	args.regions = [] if args.regions is None else list(all_regions) if len(args.regions)==0 else args.regions
	
	from datetime import datetime as dt
	import pandas as pd
	import matplotlib.pyplot as plt
	import matplotlib.dates as mdates
	import numpy as np
	
	for which in args.which:
		df = pd.read_csv(f"https://raw.githubusercontent.com/CSSEGISandData/COVID-19/master/csse_covid_19_data/csse_covid_19_time_series/time_series_19-covid-{which}.csv")
		
		if args.verbose > 0:
			unregioned_countries:list = df.loc[~df["Country/Region"].isin([c for r in all_regions.values() for c in r])]["Country/Region"].tolist()
			if len(unregioned_countries) != 0:
				_ = "\n\t"
				print(f"Unregioned countries{_}{_.join(unregioned_countries)}")
		if args.verbose > 1:
			for region, countries in all_regions.items():
				print(region)
				for country in countries:
					print(f"\t{country}")
		
		graph(df, args.scale, args.per_capita, args.countries, args.regions, args.min, args.split_regions)
