import yfinance as yf
import pandas as pd
import numpy as np
from datetime import datetime, timedelta

class SP500EqualWeightETF:
    def __init__(self):
        self.sp500_tickers = self.get_sp500_tickers()
        self.portfolio = pd.DataFrame()

    def get_sp500_tickers(self):
        # We'll use a predefined list of S&P 500 tickers
        # In a real-world scenario, you might want to fetch this list dynamically
        return ["AAPL", "MSFT", "AMZN", "GOOGL", "FB", "TSLA", "BRK-B", "JNJ", "JPM", "V", "PG", "UNH", "HD", "MA", "NVDA"]  # This is a sample, not the full 500

    def fetch_data(self, period="1mo"):
        data = yf.download(self.sp500_tickers, period=period)
        return data['Adj Close']

    def calculate_returns(self, prices):
        return prices.pct_change()

    def calculate_portfolio_value(self, initial_investment=10000):
        num_stocks = len(self.sp500_tickers)
        investment_per_stock = initial_investment / num_stocks
        
        stock_quantities = investment_per_stock / self.portfolio.iloc[0]
        portfolio_value = (self.portfolio * stock_quantities).sum(axis=1)
        
        return portfolio_value

    def calculate_metrics(self, portfolio_value):
        total_return = (portfolio_value.iloc[-1] - portfolio_value.iloc[0]) / portfolio_value.iloc[0]
        annualized_return = (1 + total_return) ** (252 / len(portfolio_value)) - 1
        
        daily_returns = portfolio_value.pct_change()
        volatility = daily_returns.std() * np.sqrt(252)
        
        sharpe_ratio = annualized_return / volatility
        
        return {
            'Total Return': f'{total_return:.2%}',
            'Annualized Return': f'{annualized_return:.2%}',
            'Volatility': f'{volatility:.2%}',
            'Sharpe Ratio': f'{sharpe_ratio:.2f}'
        }

    def run_simulation(self, initial_investment=10000, period="1mo"):
        print(f"Fetching data for {len(self.sp500_tickers)} S&P 500 stocks...")
        self.portfolio = self.fetch_data(period)
        
        print("Calculating portfolio value...")
        portfolio_value = self.calculate_portfolio_value(initial_investment)
        
        print("Calculating performance metrics...")
        metrics = self.calculate_metrics(portfolio_value)
        
        print("\nS&P 500 Equal Weight ETF Simulation Results:")
        print(f"Period: {period}")
        print(f"Initial Investment: ${initial_investment:,.2f}")
        print(f"Final Portfolio Value: ${portfolio_value.iloc[-1]:,.2f}")
        for metric, value in metrics.items():
            print(f"{metric}: {value}")

        return portfolio_value, metrics

def main():
    etf = SP500EqualWeightETF()
    etf.run_simulation(initial_investment=10000, period="1mo")

if __name__ == "__main__":
    main()