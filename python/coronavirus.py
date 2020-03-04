#!/usr/bin/env python3


# Plot countries vs infections
#df.drop(columns=["Lat","Long"]).groupby(df["Country/Region"]).aggregate("sum").plot(x="Country/Region")

drop_cols:list = ["Province/State","Country/Region","Lat","Long"]

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

region_names:tuple = (
	"Australasia",
	"China",
	"East asia excluding China",
	"Europe",
	"Middle East",
	"North America",
	"South America",
	"Sub-Saharan Africa",
	"International",
)
regions:list = (auzealand, china, asia_excl_china, european_countries, semitia, canadia, samerica, africa, international)


populations_df = None
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
	
	global populations_df
	if populations_df is not None:
		_df = populations_df.loc[populations_df["Country Name"]==region]
		try:
			return int(_df.loc[_df["Year"]==_df["Year"].max()].iat[0, 4])
		except IndexError:
			raise Exception(f"No population data for: {region}")
	try:
		populations_df = pd.read_csv("populations.csv")
	except FileNotFoundError:
		populations_df = pd.read_csv("https://pkgstore.datahub.io/core/population/population_csv/data/ead5be05591360d33ad1a37382f8f8b1/population_csv.csv")
		populations_df.to_csv("populations.csv")
	return get_population(region)



def graph_df(df, inc_legend:bool, yscale:str, per_capita:bool, china:int, only_region:str):
	for c in df["Country/Region"].values:
		to_break:bool = False
		for ls in regions:
			to_break = (c in ls)
			if to_break:
				break
		if to_break:
			continue
		
		print(c)

	fig, ax = plt.subplots()
	plt.margins(x=0,y=0)
	_region_names:list = []
	for region, region_name in zip(regions, region_names):
		if only_region is not None and region_name != only_region:
			continue
		is_china:bool = (region_name == "China")
		if china == 0 and is_china:
			continue
		if china == 2 and not is_china:
			continue
		_region_names.append(region_name)
		_df = df.loc[df["Country/Region"].isin(region)].drop(columns=drop_cols)
		arr = _df.sum(axis=0)
		arr.index = pd.to_datetime(arr.index)
		if per_capita:
			n:int = 0
			for _subregion in region:
				try:
					n += get_population(_subregion)
				except TypeError:
					# returned None
					raise Exception(f"Cannot get population for: {_subregion}")
			arr = arr.divide(n)
		arr.plot(ax=ax)
		n_infected_now:int = arr[-1]
		plt.annotate(n_infected_now, xy=(plt.gca().get_xlim()[1],n_infected_now))
	if inc_legend:
		ax.legend(_region_names)
	ax.set_yscale(yscale)

	plt.show()


if __name__ == "__main__":
	import argparse
	
	parser = argparse.ArgumentParser()
	parser.add_argument("which", nargs="*", help="Confirmed Deaths or Recovered")
	parser.add_argument("--scale", default="log", help="log or linear or symlog or logit")
	parser.add_argument("--china", type=int, default=1, help="0=exclude, 1=include, 2=only")
	parser.add_argument("--only-region", help="'"+"' '".join(region_names)+"'")
	parser.add_argument("--no-legend", dest="legend", default=True, action="store_false")
	parser.add_argument("--per-capita", default=False, action="store_true")
	args = parser.parse_args()
	
	from datetime import datetime as dt
	import pandas as pd
	import matplotlib.pyplot as plt
	import matplotlib.dates as mdates
	import numpy as np
	
	for which in args.which:
		df = pd.read_csv(f"https://raw.githubusercontent.com/CSSEGISandData/COVID-19/master/csse_covid_19_data/csse_covid_19_time_series/time_series_19-covid-{which}.csv")
		graph_df(df, args.legend, args.scale, args.per_capita, args.china, args.only_region)
