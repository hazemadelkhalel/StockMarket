"use client";
import Link from "next/link";
import { GBtn, Navbar, StockerField } from "../../components";
import "./scss/profile.css";
import { Divider } from "primereact/divider";
import "primereact/resources/themes/lara-light-blue/theme.css";

import "primereact/resources/primereact.min.css";
import "primeicons/primeicons.css";
import "primeflex/primeflex.css";

import { DataTable } from "primereact/datatable";
import { Column } from "primereact/column";
import { SetStateAction, useEffect, useRef, useState } from "react";
import { Tag } from "primereact/tag";
import { Chart } from "primereact/chart";
import { Button } from "primereact/button";
import { useRouter } from "next/navigation";
import { getSessionToken } from "../../utils/cookie";
import { Toast } from "primereact/toast";
import { get } from "http";
interface Stocker {
  name: string;
  type: string;
  company: string;
  price: string;
  balance: string;
  date: string;
}

const Profile = () => {
  const [username, setUsername] = useState("");
  const [email, setEmail] = useState("");
  const [created_at, setCreatedAt] = useState("");
  const [first_name, setFirstName] = useState("");
  const [last_name, setLastName] = useState("");
  const [phone, setPhone] = useState("");
  const [aboutme, setAboutme] = useState("");
  const [website, setWebsite] = useState("");
  const [facebook_profile, setFacebook_profile] = useState("");
  const [instagram_profile, setInstagram_profile] = useState("");
  const [card_number, setCard_number] = useState("");
  const [wallet, setWallet] = useState(0);

  const [totalBuy, setTotalBuy] = useState(0);
  const [totalSell, setTotalSell] = useState(0);
  const [selectedTap, setSelectedTap] = useState(1);
  const [stockers, setStockers] = useState<Stocker[]>([]);
  const [chartData, setChartData] = useState({});
  const [chartOptions, setChartOptions] = useState({});
  const [timeRange, setTimeRange] = useState("day"); // 'day', 'month', 'year'
  const router = useRouter();
  const sessionToken = getSessionToken();
  const toast = useRef(null);

  console.log(sessionToken);
  if (!sessionToken) {
    router.push("/login");
    return;
  }

  const getSeverity = (stocker: Stocker) => {
    switch (stocker.type) {
      case "Buy":
        return "success";
      default:
        return "danger";
    }
  };
  useEffect(() => {
    const transactions: Stocker[] = [
      {
        name: "Transaction 1",
        type: "Buy",
        company: "Apple.inc",
        price: "100$",
        balance: "1245$",
        date: "12/12/2020",
      },
      {
        name: "Transaction 2",
        type: "Sell",
        company: "Microsoft",
        price: "150$",
        balance: "2000$",
        date: "25/05/2021",
      },
      {
        name: "Transaction 3",
        type: "Buy",
        company: "Google",
        price: "200$",
        balance: "1500$",
        date: "09/03/2022",
      },
      {
        name: "Transaction 4",
        type: "Sell",
        company: "Amazon",
        price: "250$",
        balance: "1000$",
        date: "18/11/2021",
      },
      {
        name: "Transaction 5",
        type: "Buy",
        company: "Tesla",
        price: "300$",
        balance: "2500$",
        date: "08/07/2020",
      },
      {
        name: "Transaction 6",
        type: "Sell",
        company: "Apple.inc",
        price: "150$",
        balance: "1200$",
        date: "14/03/2022",
      },
      {
        name: "Transaction 7",
        type: "Buy",
        company: "Google",
        price: "120$",
        balance: "1800$",
        date: "30/06/2020",
      },
      {
        name: "Transaction 8",
        type: "Sell",
        company: "Microsoft",
        price: "180$",
        balance: "2200$",
        date: "02/09/2022",
      },
      {
        name: "Transaction 9",
        type: "Buy",
        company: "Amazon",
        price: "220$",
        balance: "1600$",
        date: "22/10/2021",
      },
      {
        name: "Transaction 10",
        type: "Sell",
        company: "Tesla",
        price: "280$",
        balance: "2300$",
        date: "07/05/2020",
      },
      {
        name: "Transaction 11",
        type: "Buy",
        company: "Apple.inc",
        price: "90$",
        balance: "1400$",
        date: "28/01/2021",
      },
      {
        name: "Transaction 12",
        type: "Sell",
        company: "Google",
        price: "170$",
        balance: "1900$",
        date: "17/04/2022",
      },
      {
        name: "Transaction 13",
        type: "Buy",
        company: "Microsoft",
        price: "110$",
        balance: "2100$",
        date: "14/09/2020",
      },
      {
        name: "Transaction 14",
        type: "Sell",
        company: "Amazon",
        price: "190$",
        balance: "1300$",
        date: "12/03/2021",
      },
      {
        name: "Transaction 15",
        type: "Buy",
        company: "Tesla",
        price: "260$",
        balance: "2400$",
        date: "19/08/2020",
      },
      {
        name: "Transaction 16",
        type: "Sell",
        company: "Apple.inc",
        price: "130$",
        balance: "2000$",
        date: "02/02/2022",
      },
      {
        name: "Transaction 17",
        type: "Buy",
        company: "Google",
        price: "240$",
        balance: "1700$",
        date: "10/11/2021",
      },
      {
        name: "Transaction 18",
        type: "Sell",
        company: "Microsoft",
        price: "200$",
        balance: "1600$",
        date: "26/06/2020",
      },
      {
        name: "Transaction 19",
        type: "Buy",
        company: "Amazon",
        price: "140$",
        balance: "1800$",
        date: "11/09/2021",
      },
      {
        name: "Transaction 20",
        type: "Sell",
        company: "Tesla",
        price: "160$",
        balance: "2100$",
        date: "30/04/2022",
      },
    ];
    setStockers(transactions);

    const documentStyle = getComputedStyle(document.documentElement);
    const textColor = documentStyle.getPropertyValue("--text-color");
    const textColorSecondary = documentStyle.getPropertyValue(
      "--text-color-secondary"
    );
    const surfaceBorder = documentStyle.getPropertyValue("--surface-border");
    const dataPoints =
      timeRange === "day" ? 48 : timeRange === "month" ? 30 : 12;

    const monthNames = [
      "January",
      "February",
      "March",
      "April",
      "May",
      "June",
      "July",
      "August",
      "September",
      "October",
      "November",
      "December",
    ];

    const data = {
      labels: Array.from({ length: dataPoints }, (_, i) => {
        if (timeRange === "day") {
          const hour = Math.floor(i / 2);
          const minute = i % 2 === 0 ? "00" : "30";
          return `${hour}:${minute}`;
        } else if (timeRange === "month") {
          return `Day ${i + 1}`;
        } else {
          return monthNames[i];
        }
      }),
      datasets: [
        {
          label: "First Dataset",
          data: Array.from({ length: dataPoints }, () =>
            Math.floor(Math.random() * 100)
          ),
          fill: true,
          borderColor: documentStyle.getPropertyValue("--blue-500"),
        },
      ],
    };

    const options = {
      maintainAspectRatio: false,
      aspectRatio: 0.6,
      plugins: {
        legend: {
          labels: {
            color: textColor,
          },
        },
      },
      scales: {
        x: {
          ticks: {
            color: textColorSecondary,
          },
          grid: {
            color: surfaceBorder,
          },
        },
        y: {
          ticks: {
            color: textColorSecondary,
          },
          grid: {
            color: surfaceBorder,
          },
        },
      },
    };

    setChartData(data);
    setChartOptions(options);
  }, [timeRange]);

  const getUser = async () => {
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
        (toast.current as any)?.show({
          severity: "error",
          summary: "Failed",
          detail: data.error,
          life: 3000,
        });
        return;
      }

      setUsername(data["User"].username);
      setEmail(data["User"].email);
      setCreatedAt(data["User"].created_at);
      setFirstName(data["User"].first_name);
      setLastName(data["User"].last_name);
      setPhone(data["User"].phone);
      setAboutme(data["User"].aboutme);
      setWebsite(data["User"].website);
      setFacebook_profile(data["User"].facebook_profile);
      setInstagram_profile(data["User"].instagram_profile);
      setCard_number(data["User"].card_number);
      setWallet(data["User"].wallet);
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  };

  const getStockers = async () => {
    const response = await fetch(
      `http://localhost:8001/api/profile/getAllTransactions?token=${getSessionToken()}`,
      {
        method: "GET",
        headers: {
          "Content-Type": "application/json",
        },
      }
    );

    const data = await response.json();
    if (data.error) {
      (toast.current as any)?.show({
        severity: "error",
        summary: "Failed",
        detail: data.error,
        life: 3000,
      });
      return;
    }
    let transactions: Stocker[] = [];

    console.log(data["transactions"]);
    let cntBuy = 0;
    let cntSell = 0;
    for (let i = 0; i < data["transactions"]?.length; i++) {
      let transaction = data["transactions"][i];
      transactions.push({
        name: "Transaction #" + transaction.id,
        type: transaction.stock.type,
        company: transaction.stock.company,
        price: parseFloat(transaction.stock.price).toFixed(2),
        balance: parseFloat(transaction.user.wallet).toFixed(2),
        date: transaction.date,
      });
      if (transaction.stock.type == "Buy") cntBuy++;
      else cntSell++;
      setTotalBuy(cntBuy);
      setTotalSell(cntSell);
      transactions[i].date = transactions[i].date
        .replace("T", " ")
        .split(".")[0];
    }
    setStockers(transactions);
  };
  useEffect(() => {
    getUser();
    getStockers();
  }, [selectedTap]);

  const statusBodyTemplate = (stocker: Stocker) => {
    return (
      <Tag
        value={stocker.type}
        severity={getSeverity(stocker)}
        style={{ padding: "3px 15px" }}
      ></Tag>
    );
  };

  const dateBodyTemplate = (rowData: { date: string }) => {
    return <span>{rowData.date}</span>;
  };

  const handleTimeRangeChange = (newTimeRange: SetStateAction<string>) => {
    setTimeRange(newTimeRange);
  };
  return (
    <>
      <Navbar idx={0} />
      <Toast ref={toast} />
      <div className="profile-container">
        <section className="profile-section-1">
          <div className="profile-pc">H</div>
          <div className="profile-info">
            <div className="p-i-data">
              <h1>hazemadelkhalel</h1>
              <div className="p-i-creation">
                <img src="/SVG/calendar.svg" />
                <span>Joined 6 months ago.</span>
              </div>
            </div>
            <div className="p-i-container-progress">
              <div className="p-i-progress">
                <h3>{totalBuy}</h3>
                <span>Buy</span>
              </div>
              <div className="p-i-progress">
                <h3>{totalSell}</h3>
                <span>Sell</span>
              </div>
              <div className="p-i-progress">
                <h3>{stockers.length}</h3>
                <span>Transactions</span>
              </div>
              <div className="p-i-progress">
                <h3>3</h3>
                <span>Stocks</span>
              </div>
            </div>
          </div>
        </section>
        <div style={{ overflow: "hidden" }}>
          <Divider />
        </div>
        <section className="profile-section-2">
          <div className="p-s-2-header">
            <Link href="/profile/#mystocks" scroll={false}>
              <h2
                className={`${
                  selectedTap == 1 ? "nav-link active" : "nav-link "
                }`}
                onClick={() => {
                  setSelectedTap(1);
                }}
              >
                My Stocks
              </h2>
            </Link>
            <Link href="/profile/#history" scroll={false}>
              <h2
                className={`${
                  selectedTap == 2 ? "nav-link active" : "nav-link "
                }`}
                onClick={() => {
                  setSelectedTap(2);
                }}
              >
                History
              </h2>
            </Link>
            <Link href="/profile/#settings" scroll={false}>
              <h2
                className={`${
                  selectedTap == 3 ? "nav-link active" : "nav-link "
                }`}
                onClick={() => {
                  setSelectedTap(3);
                }}
              >
                Settings
              </h2>
            </Link>
          </div>
        </section>
        <div style={{ overflow: "hidden" }}>
          <Divider />
        </div>
        <div className="profile-section-3">
          {selectedTap == 1 ? (
            <div style={{ width: "100%" }}>
              <Chart type="line" data={chartData} options={chartOptions} />
              <Button
                label="Day"
                onClick={() => handleTimeRangeChange("day")}
                style={{
                  marginLeft: "30px",
                  marginRight: "10px",
                  marginTop: "20px",
                  fontSize: "14px",
                  padding: "10px 15px",
                }}
              />
              <Button
                label="Month"
                onClick={() => handleTimeRangeChange("month")}
                style={{
                  marginRight: "10px",
                  fontSize: "14px",
                  padding: "10px 15px",
                }}
              />
              <Button
                label="Year"
                onClick={() => handleTimeRangeChange("year")}
                style={{
                  fontSize: "14px",
                  padding: "10px 15px",
                }}
              />
            </div>
          ) : null}
          {selectedTap == 2 ? (
            <div style={{ width: "100%" }}>
              <DataTable
                value={stockers}
                stripedRows
                paginator
                sortMode="multiple"
                removableSort
                rows={10}
                rowsPerPageOptions={[10, 50, 100, 200]}
              >
                <Column field="name" sortable header="Name"></Column>
                <Column field="company" sortable header="Company"></Column>
                <Column field="price" sortable header="Price "></Column>
                <Column field="balance" sortable header="Balance "></Column>
                <Column
                  field="type"
                  header="Type"
                  sortable
                  body={statusBodyTemplate}
                ></Column>
                <Column
                  field="date"
                  sortable
                  header="Date"
                  body={dateBodyTemplate}
                ></Column>
              </DataTable>
            </div>
          ) : null}
          {selectedTap == 3 ? (
            <div className="p-s-3-settings-container">
              <div className="settings-left">
                <h1>Public Info</h1>
                <p>
                  This information will be publicly displayed and visible for
                  all users.
                </p>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Username</span>
                    <input
                      type="text"
                      placeholder="Username"
                      value={username}
                      disabled
                    />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">About Me</span>
                    <textarea placeholder="" value={aboutme} />
                  </div>
                </div>
                <h3>
                  Links to your social media are visible under every idea or
                  script you publish, depending on your subscription type.
                </h3>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Website</span>
                    <input type="text" placeholder="Website" value={website} />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Facebook profile</span>
                    <input
                      type="text"
                      placeholder="Facebook Username"
                      value={facebook_profile}
                    />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Instagram profile</span>
                    <input
                      type="text"
                      placeholder="Instagram Username"
                      value={instagram_profile}
                    />
                  </div>
                </div>
                <div className="settings-btn">
                  <GBtn btnText="Save changes" clickEvent={() => {}} />
                </div>
              </div>
              <div className="settings-right">
                <h1>Private Details</h1>
                <p>This information will not be publicly displayed.</p>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Email</span>
                    <input type="text" placeholder="Email" value={email} />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">First Name</span>
                    <input
                      type="text"
                      placeholder="First Name"
                      value={first_name}
                    />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Last Name</span>
                    <input
                      type="text"
                      placeholder="Last Name"
                      value={last_name}
                    />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Phone</span>
                    <input type="text" placeholder="Phone" value={phone} />
                  </div>
                </div>
                <Divider />
                <h1 style={{ marginTop: "20px" }}>
                  Billing and Credit Details
                </h1>
                <p>
                  This information will be used for billing purposes and payment
                  of your subscription.
                </p>
                <div className="settings-item">
                  <div className="settings-item">
                    <div className="stocker-item-input">
                      <span className="gi-item-text">Credit Number</span>
                      <input
                        type="text"
                        placeholder="Credit Number"
                        value={card_number}
                      />
                    </div>
                  </div>
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Wallet</span>
                    <span
                      className="gi-item-text"
                      style={{
                        fontWeight: "bold",
                        color: "black",
                        fontSize: 13,
                        width: "100%",
                      }}
                    >
                      ${wallet.toFixed(2)}
                    </span>
                  </div>
                </div>
                <div className="settings-btn">
                  <GBtn btnText="Save changes" clickEvent={() => {}} />
                </div>
              </div>
            </div>
          ) : null}
        </div>
      </div>
    </>
  );
};

export default Profile;
