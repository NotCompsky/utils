#!/usr/bin/env python3


'''
CREATE DATABASE finance;
USE income;

CREATE TABLE income (
	
);
'''


class Income:
	def __init__(self, income_type, source


income_tax_bands__uk:tuple = (
	(0,      0.0),
	(12570,  0.2),
	(50000,  0.4), # If income is 50,000 or less, then taxed at 0.2
	(150000, 0.45),
)


def calc_progressive_tax(bands:tuple, income:int, tax_free_allowance_deduction=0):
	income_tax_bands__actual:list = [(x[0] - tax_free_allowance_deduction, x[1]) for x in bands if x[0] >= tax_free_allowance_deduction]
	
	tax_payable_in_prev_bands:float = sum([
		(x[1][0] - x[0][0])*x[0][1]
		for x in zip(income_tax_bands__actual[:-1], income_tax_bands__actual[1:])
		if x[1][0] <= income
	])
	taxable_income_in_current_band:float = income - sum([(x[1][0] - x[0][0]) for x in zip(income_tax_bands__actual[:-1], income_tax_bands__actual[1:]) if x[1][0] <= income])
	
	tax_payable_in_current_band:float = taxable_income_in_current_band * [x[1] for x in income_tax_bands__actual if x[0]<income][-1]
	
	return tax_payable_in_current_band + tax_payable_in_prev_bands


def get_patreon_income():
	return 0


def calc_other_sources_of_personal_income():
	return 0


def calc_income_tax__england(income:int):
	return calc_progressive_tax(income_tax_bands__uk,  income,  min(bands[1][0], max(0, (income - 100000)) / 2))


def calc_wage_posttax__uk(wage:int):
	return wage


def calc_dividend_tax__uk(pretax:int):
	return 0


def calc_dividend__uk(corporate_posttax_profit:int):
	return corporate_posttax_profit - calc_dividend_tax__uk(corporate_posttax_profit)


def calc_corporate_tax__uk(pretax_profit:int):
	return 0


def calc_corporate_profit__uk(revenue:int, wage:int):
	pretax_profit:int = revenue - wage
	return pretax_profit - calc_corporate_profit__uk(pretax_profit)


def calc_income(corporate_revenue:int):
	wage:int = 8000
	income:int = wage + calc_other_sources_of_personal_income()
	return calc_dividend__uk(calc_corporate_profit__uk(corporate_revenue, wage)) + income - calc_income_tax__england(income)


if __name__ == "__main__":
	import argparse
	
	parser = argparse.ArgumentParser()
	parser.add_argument("--foo")
	args = parser.parse_args()
