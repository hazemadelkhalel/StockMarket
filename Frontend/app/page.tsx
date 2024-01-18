"use client";
import { Footer, Navbar } from "./components";
import "./scss/home.css";
import {
  setSessionToken,
  getSessionToken,
  removeSessionToken,
} from "../app/utils/cookie";
import { useRouter } from "next/navigation"; // Import the correct module
import { useEffect, useState } from "react";
import { Redirect } from "next";
import { Button } from "primereact/button";

import "primereact/resources/themes/lara-light-blue/theme.css";
import "primereact/resources/primereact.min.css";
import "primeicons/primeicons.css";
import { TabPanel, TabView } from "primereact/tabview";
import { Card } from "primereact/card";
import { Carousel, CarouselResponsiveOption } from "primereact/carousel";
import { Chart } from "primereact/chart";

interface Wisdom {
  title: string;
  description: string;
}

export default function Home() {
  const router = useRouter();
  const [username, setUsername] = useState("");
  const [chartData, setChartData] = useState({});
  const [chartOptions, setChartOptions] = useState({});
  const [profit, setProfit] = useState(0);
  const [price, setPrice] = useState(0);
  const [available_stocks, setAvailableStocks] = useState(0);
  const wisdom: Wisdom[] = [
    {
      title: "Warren Buffett",
      description:
        "The stock market is a device for transferring money from the impatient to the patient.",
    },
    {
      title: "Peter Lynch",
      description:
        "In the short run, the market is a voting machine, but in the long run, it is a weighing machine.",
    },
    {
      title: "Benjamin Graham",
      description:
        "The stock market is filled with individuals who know the price of everything, but the value of nothing.",
    },
    {
      title: "John C. Bogle",
      description:
        "Don't look for the needle in the haystack. Just buy the haystack!",
    },
    {
      title: "George Soros",
      description:
        "It's not whether you're right or wrong that's important, but how much money you make when you're right and how much you lose when you're wrong.",
    },
  ];

  const responsiveOptions: CarouselResponsiveOption[] = [
    {
      breakpoint: "1400px",
      numVisible: 2,
      numScroll: 1,
    },
    {
      breakpoint: "1199px",
      numVisible: 3,
      numScroll: 1,
    },
    {
      breakpoint: "767px",
      numVisible: 2,
      numScroll: 1,
    },
    {
      breakpoint: "575px",
      numVisible: 1,
      numScroll: 1,
    },
  ];

  if (getSessionToken() === "") {
    router.push("/login");
    return <h1>Redirecting...</h1>; // or you can render a loading state
  }
  const fetchWallet = async () => {
    try {
      const response = await fetch(
        `http://localhost:8001/api/getUser?token=${getSessionToken()}`,
        {
          method: "GET",
          headers: {
            "Content-Type": "application/json",
          },
        }
      );

      const data = await response.json();
      if (data.error) {
        console.log(data.error);
        return;
      }
      setUsername(data["User"].username);
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  };
  useEffect(() => {
    fetchWallet();
  }, []);

  const wisdomTemplate = (wisdom: Wisdom) => {
    return (
      <div className="flex flex-col border-1 surface-border border-round pt-4 pb-3 px-3 h-full items-center justify-center card">
        <h2 className="text-xl  mb-1">{wisdom.title}</h2>
        <div className="text-center flex flex-wrap flex-col">
          <p className="text-m mt-0 mb-3 text-gray-400 ">
            {wisdom.description}
          </p>
        </div>
      </div>
    );
  };

  useEffect(() => {
    const socket = new WebSocket("ws://127.0.0.1:8083");

    socket.onopen = () => {
      console.log("WebSocket connection opened");

      // Send an initial message
      socket.send("Hello, server!");

      // Set up interval to send a message every 5 seconds
      const intervalId = setInterval(() => {
        socket.send("Sending a message at regular intervals");
      }, 5000);

      // Save the interval ID for cleanup
      return () => clearInterval(intervalId);
    };

    socket.onmessage = (event) => {
      console.log(`Received message from server: ${event.data}`);
      const data = JSON.parse(event.data);
      let companies: string[] = [];
      let profits: number[] = [];
      let prices: number[] = [];
      let available_stocks_arr: number[] = [];

      for (let key in data) {
        companies.push(data[key].company);
        profits.push(data[key].profit);
        prices.push(data[key].current_price);
        available_stocks_arr.push(data[key].available_stocks);
      }

      const documentStyle = getComputedStyle(document.documentElement);
      const textColor = documentStyle.getPropertyValue("--text-color");
      const textColorSecondary = documentStyle.getPropertyValue(
        "--text-color-secondary"
      );
      const surfaceBorder = documentStyle.getPropertyValue("--surface-border");
      const dataChart = {
        labels: companies,
        datasets: [
          {
            label: "Profit",
            backgroundColor: documentStyle.getPropertyValue("--blue-500"),
            borderColor: documentStyle.getPropertyValue("--blue-500"),
            data: profits,
          },
          {
            label: "Price",
            backgroundColor: documentStyle.getPropertyValue("--pink-500"),
            borderColor: documentStyle.getPropertyValue("--pink-500"),
            data: prices,
          },
          {
            label: "Available Stocks",
            backgroundColor: documentStyle.getPropertyValue("--yellow-500"),
            borderColor: documentStyle.getPropertyValue("--yellow-500"),
            data: available_stocks,
          },
        ],
      };
      const options = {
        maintainAspectRatio: false,
        aspectRatio: 0.8,
        plugins: {
          legend: {
            labels: {
              fontColor: textColor,
            },
          },
        },
        scales: {
          x: {
            ticks: {
              color: textColorSecondary,
              font: {
                weight: 500,
              },
            },
            grid: {
              display: false,
              drawBorder: false,
            },
          },
          y: {
            ticks: {
              color: textColorSecondary,
            },
            grid: {
              color: surfaceBorder,
              drawBorder: false,
            },
          },
        },
      };

      setChartData(dataChart);
      setChartOptions(options);
    };

    return () => {
      socket.close();
    };
  }, []);
  return (
    <main className="home-container">
      <Navbar idx={0} username={username} />
      <div className="home-content">
        <div className="home-section1">
          <h1>Stock Market</h1>
          <h2>Invest with confidence, trade with ease.</h2>
          <div className="get-started-btn">
            <Button
              label="Get Started"
              className="p-button-raised p-button-rounded"
              onClick={() => router.push("/market")}
            />
          </div>
        </div>
        <div className="home-section2">
          <h1>Stock Wisdom</h1>
          <div style={{ width: "90%" }}>
            <Carousel
              value={wisdom}
              numVisible={3}
              numScroll={3}
              responsiveOptions={responsiveOptions}
              itemTemplate={wisdomTemplate}
            />
          </div>
        </div>
        <div id="top-companies" className="top-companies">
          <h1>Top Companies</h1>
          <Chart
            type="line"
            data={chartData}
            options={chartOptions}
            style={{ width: "90%" }}
          />
        </div>

        <div className="home-section3">
          <h1>What's Benifts?</h1>
          <TabView style={{ width: "90%" }}>
            <TabPanel header="Market Access and Trading">
              <p className="m-0 text-xl">
                The stock market provides investors with a platform to buy and
                sell financial instruments such as stocks, bonds, and
                derivatives. One of its key features is the ease of market
                access, allowing participants to engage in trading activities.
                Through brokerage accounts and trading platforms, investors can
                execute trades in real-time, reacting to market fluctuations and
                news. This accessibility fosters liquidity, contributing to the
                efficient functioning of the market.
              </p>
            </TabPanel>
            <TabPanel header="Information Transparency and Analysis Tools">
              <p className="m-0 text-xl">
                The stock market offers a wealth of information to participants,
                promoting transparency. Companies listed on stock exchanges are
                required to disclose financial reports and other relevant
                information, enabling investors to make informed decisions.
                Additionally, various analysis tools and financial indicators
                are available to help investors assess the performance and
                potential risks of different stocks. These tools include
                price-to-earnings ratios, technical analysis charts, and
                fundamental analysis metrics, empowering investors to conduct
                thorough research.
              </p>
            </TabPanel>
            <TabPanel header="Diversification and Investment Opportunities">
              <p className="m-0 text-xl">
                Another notable feature of the stock market is its role in
                providing opportunities for portfolio diversification. Investors
                can allocate their funds across different industries, sectors,
                and geographical regions by investing in a variety of stocks.
                This diversification strategy helps mitigate risk and enhance
                the potential for returns. Moreover, the stock market serves as
                a channel for companies to raise capital by issuing shares,
                facilitating economic growth and innovation. This dynamic
                environment presents investors with a range of investment
                opportunities, from stable dividend-paying stocks to
                high-growth, emerging market securities.
              </p>
            </TabPanel>
          </TabView>
        </div>
        <div className="home-section4">
          <h1>How to Start?</h1>
          <div
            id="help"
            className="flex flex-wrap justify-center"
            style={{ width: "90%" }}
          >
            <Card
              title="1. Create an Account"
              subTitle="Sign up for a free account"
              className="card"
            >
              <p className="m-0">
                To get started, sign up for a free account. You will need to
                provide your name, email address, and a password to create an
                account. After creating an account, you will receive a
                confirmation email. Click on the link in the email to activate
                your account.
              </p>
            </Card>
            <Card
              title="2. Fund Your Account"
              subTitle="Deposit funds into your account"
              className="card"
            >
              <p className="m-0">
                Once your account is activated, you can deposit funds into your
                account. You can deposit funds using a bank transfer or a
                credit/debit card. Once you have deposited funds into your
                account, you can start trading.
              </p>
            </Card>
            <Card
              title="3. Start Trading"
              subTitle="Buy and sell stocks"
              className="card"
            >
              <p className="m-0">
                You can start trading by searching for a stock and clicking on
                the buy button. You can also sell stocks by clicking on the sell
                button. You can view your portfolio and transaction history by
                clicking on the portfolio button.
              </p>
            </Card>
          </div>
        </div>
      </div>
      <Footer />
    </main>
  );
}
