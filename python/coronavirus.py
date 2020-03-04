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

def graph_arr(ax, arr, per_capita:bool, region_list:list):
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
	arr.plot(ax=ax)
	n_infected_now:int = arr[-1]
	plt.annotate(n_infected_now, xy=(plt.gca().get_xlim()[1],n_infected_now))


def graph(df, inc_legend:bool, yscale:str, per_capita:bool, countries:list, regions:list):
	drop_cols:list = ["Province/State","Country/Region","Lat","Long"]
	fig, ax = plt.subplots()
	plt.margins(x=0,y=0)
	_region_names:list = []
	for region in regions:
		_region_names.append(region)
		_df = df.loc[df["Country/Region"].isin(all_regions[region])].drop(columns=drop_cols)
		graph_arr(ax, _df.sum(axis=0), per_capita, all_regions[region])
	for country in countries:
		_region_names.append(country)
		_df = df.loc[df["Country/Region"]==country].drop(columns=drop_cols)
		graph_arr(ax, _df.sum(axis=0), per_capita, [country])
	if inc_legend:
		ax.legend(_region_names)
	ax.set_yscale(yscale)

	plt.show()


if __name__ == "__main__":
	import argparse
	
	parser = argparse.ArgumentParser()
	parser.add_argument("which", nargs="*", help="Confirmed Deaths or Recovered")
	parser.add_argument("--scale", default="log", help="log or linear or symlog or logit")
	parser.add_argument("-r","--regions", nargs="*", help="Regions to graph. If none specified, all included. Options: '"+"' '".join(list(all_regions))+"'")
	parser.add_argument("-c","--countries", nargs="*", help="Countries to graph. If none specified, all included.")
	parser.add_argument("--no-legend", dest="legend", default=True, action="store_false")
	parser.add_argument("--per-capita", default=False, action="store_true")
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
		graph(df, args.legend, args.scale, args.per_capita, args.countries, args.regions)
