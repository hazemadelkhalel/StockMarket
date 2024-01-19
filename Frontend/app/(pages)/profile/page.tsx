"use client";
import Link from "next/link";
import { Footer, GBtn, Navbar, StockerField } from "../../components";
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
import { getSessionToken, removeSessionToken } from "../../utils/cookie";
import { Toast } from "primereact/toast";
import { get } from "http";
interface Stocker {
  name: string;
  type: string;
  company: string;
  price: string;
  balance: string;
  transaction_date: string;
  quantity: number;
  available_stocks: number;
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
  const [timeRange, setTimeRange] = useState("day"); // 'day', 'month', 'year'
  const router = useRouter();
  const sessionToken = getSessionToken();
  const toast = useRef(null);
  const [walletValues, setWalletValues] = useState<number[]>([]);
  const [sumWalletValues, setSumWalletValues] = useState<number>(0);
  const [stockCart, setStockCart] = useState<Stocker[]>([]);

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

  const handleSaveButton = async () => {
    const response = await fetch("http://localhost:8001/api/profile/update", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        token: getSessionToken(),
        user: {
          username: username,
          email: email,
          first_name: first_name,
          last_name: last_name,
          phone: phone,
          aboutme: aboutme,
          website: website,
          facebook_profile: facebook_profile,
          instagram_profile: instagram_profile,
          card_number: card_number,
          wallet: wallet,
        },
      }),
    });

    const data = await response.json();
    if (data.error) {
      (toast.current as any)?.show({
        severity: "error",
        summary: "Failed",
        detail: data.error,
        life: 1500,
      });
      if (data.error == "Invalid token") {
        removeSessionToken();
        router.push("/login");
      }
      return;
    }

    setUsername(data["user"].username);
    setEmail(data["user"].email);
    setCreatedAt(data["user"].created_at);
    setFirstName(data["user"].first_name);
    setLastName(data["user"].last_name);
    setPhone(data["user"].phone);
    setAboutme(data["user"].aboutme);
    setWebsite(data["user"].website);
    setFacebook_profile(data["user"].facebook_profile);
    setInstagram_profile(data["user"].instagram_profile);
    setCard_number(data["user"].card_number);
    setWallet(data["user"].wallet);
    (toast.current as any)?.show({
      severity: "success",
      summary: "Successful",
      detail: data.message,
      life: 1000,
    });
  };

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
          life: 1500,
        });
        if (data.error == "Invalid token") {
          removeSessionToken();
          router.push("/login");
        }
        return;
      }

      setUsername(data["user"].username);
      setEmail(data["user"].email);
      setCreatedAt(data["user"].created_at);
      setFirstName(data["user"].first_name);
      setLastName(data["user"].last_name);
      setPhone(data["user"].phone);
      setAboutme(data["user"].aboutme);
      setWebsite(data["user"].website);
      setFacebook_profile(data["user"].facebook_profile);
      setInstagram_profile(data["user"].instagram_profile);
      setCard_number(data["user"].card_number);
      setWallet(data["user"].wallet);
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  };

  const getAllTransactions = async () => {
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
        life: 1500,
      });
      if (data.error == "Invalid token") {
        removeSessionToken();
        router.push("/login");
      }
      return;
    }
    let transactions: Stocker[] = [];
    let cntBuy = 0;
    let cntSell = 0;
    let _walletValues: number[] = [];
    let _sumWalletValues = 0;
    for (let i = 0; i < data["transactions"]?.length; i++) {
      let transaction = data["transactions"][i];
      _walletValues.push(transaction.balance);
      _sumWalletValues += transaction.balance;
      transactions.push({
        name: "Transaction #" + transaction.id,
        balance: parseFloat(transaction.balance).toFixed(2),
        company: transaction.company,
        price: parseFloat(transaction.price).toFixed(2),
        type: transaction.type,
        transaction_date: transaction.transaction_date,
        quantity: transaction.quantity,
        available_stocks: 0,
      });
      if (transaction.type == "Buy") cntBuy++;
      else cntSell++;
      setTotalBuy(cntBuy);
      setTotalSell(cntSell);
    }
    setWalletValues(_walletValues);
    setStockers(transactions);
    setSumWalletValues(_sumWalletValues);
  };

  const fetchStockCart = async () => {
    const url = `http://localhost:8001/api/market/getStockCart?token=${getSessionToken()}`;

    const response = await fetch(url, {
      method: "GET",
      headers: {
        "Content-Type": "application/json",
      },
    });

    const data = await response.json();
    if (data.error) {
      console.log(data.error);
      if (data.error == "Invalid token") {
        removeSessionToken();
        router.push("/login");
      }
      return;
    }
    // setSellStocks(data["stocks"]);
    let _stockCart: Stocker[] = [];
    for (let key in data["stocks"]) {
      let curStock: Stocker = data["stocks"][key];
      _stockCart.push(curStock);
    }
    console.log("HAMADADADA", _stockCart);
    setStockCart(_stockCart);
  };

  useEffect(() => {
    getUser();
    getAllTransactions();
    fetchStockCart();
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
  // Event handler to update the state when the input value changes
  const handleWebsiteChange = (event: { target: { value: any } }) => {
    setWebsite(event.target.value);
  };
  const handleEmailChange = (event: { target: { value: any } }) => {
    setEmail(event.target.value);
  };
  const handleFirstNameChange = (event: { target: { value: any } }) => {
    setFirstName(event.target.value);
  };
  const handleLastNameChange = (event: { target: { value: any } }) => {
    setLastName(event.target.value);
  };
  const handlePhoneChange = (event: { target: { value: any } }) => {
    setPhone(event.target.value);
  };
  const handleAboutMeChange = (event: { target: { value: any } }) => {
    setAboutme(event.target.value);
  };
  const handleFacebookChange = (event: { target: { value: any } }) => {
    setFacebook_profile(event.target.value);
  };
  const handleInstagramChange = (event: { target: { value: any } }) => {
    setInstagram_profile(event.target.value);
  };
  const handleCardNumberChange = (event: { target: { value: any } }) => {
    setCard_number(event.target.value);
  };

  const formatDate = (curDate: string) => {
    const currentDate = new Date();
    const date = new Date(curDate);

    const diffTime = Math.abs(currentDate.getTime() - date.getTime());
    const diffDays = Math.floor(diffTime / (1000 * 60 * 60 * 24));
    const diffHours = Math.floor(diffTime / (1000 * 60 * 60));
    const diffMinutes = Math.floor(diffTime / (1000 * 60));
    const diffSeconds = Math.floor(diffTime / 1000);

    if (diffSeconds < 60) return <span>{diffSeconds} seconds ago</span>;
    if (diffMinutes < 60) return <span>{diffMinutes} minutes ago</span>;
    if (diffHours < 24) return <span>{diffHours} hours ago</span>;

    if (diffDays == 0) {
      return <span>Today</span>;
    }
    if (diffDays == 1) {
      return <span>Yesterday</span>;
    }
    if (diffDays < 7) {
      return <span>{diffDays} days ago</span>;
    }
    if (diffDays < 30) {
      return <span>{Math.floor(diffDays / 7)} weeks ago</span>;
    }
    if (diffDays < 365) {
      return <span>{Math.floor(diffDays / 30)} months ago</span>;
    }
    if (diffDays >= 365) {
      return <span>{Math.floor(diffDays / 365)} years ago</span>;
    }
  };

  const dateBodyTemplate = (rowData: { transaction_date: string }) => {
    return formatDate(rowData.transaction_date);
  };

  const handleTimeRangeChange = (newTimeRange: SetStateAction<string>) => {
    setTimeRange(newTimeRange);
  };
  return (
    <>
      <Navbar idx={-1} username={username} />
      <Toast ref={toast} />
      <div className="profile-container">
        <section className="profile-section-1">
          <div className="profile-pc">
            {" "}
            {username && username.length > 0 ? username[0].toUpperCase() : ""}
          </div>
          <div className="profile-info">
            <div className="p-i-data">
              <h1>{username}</h1>
              <div className="p-i-creation">
                <img src="/SVG/calendar.svg" />
                <span>Joined {formatDate(created_at)}.</span>
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
                <h3>
                  $
                  {walletValues.length == 0
                    ? 0
                    : (sumWalletValues / walletValues.length).toFixed(2)}
                </h3>
                <span>Avg. Prices</span>
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
            <div className="profile-stockCart">
              {stockCart.length > 0 ? (
                stockCart.map((stocker, index) => {
                  return (
                    <div className="profile-stockCart-item">
                      <h2>{stocker.available_stocks}</h2>
                      <p>{stocker.company}</p>
                    </div>
                  );
                })
              ) : (
                <div></div>
              )}
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
                <Column field="quantity" sortable header="Quantity "></Column>
                <Column
                  field="type"
                  header="Type"
                  sortable
                  body={statusBodyTemplate}
                ></Column>
                <Column
                  field="transaction_date"
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
                    <textarea
                      name="aboutme"
                      id="aboutme"
                      onChange={handleAboutMeChange}
                      value={aboutme}
                    />
                  </div>
                </div>
                <h3>
                  Links to your social media are visible under every idea or
                  script you publish, depending on your subscription type.
                </h3>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Website</span>
                    <input
                      type="text"
                      placeholder="Website"
                      value={website}
                      onChange={handleWebsiteChange}
                    />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Facebook profile</span>
                    <input
                      type="text"
                      placeholder="Facebook Username"
                      value={facebook_profile}
                      onChange={handleFacebookChange}
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
                      onChange={handleInstagramChange}
                    />
                  </div>
                </div>
                <div className="settings-btn">
                  <GBtn btnText="Save changes" clickEvent={handleSaveButton} />
                </div>
              </div>
              <div className="settings-right">
                <h1>Private Details</h1>
                <p>This information will not be publicly displayed.</p>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Email</span>
                    <input
                      type="text"
                      placeholder="Email"
                      value={email}
                      onChange={handleEmailChange}
                    />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">First Name</span>
                    <input
                      type="text"
                      placeholder="First Name"
                      value={first_name}
                      onChange={handleFirstNameChange}
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
                      onChange={handleLastNameChange}
                    />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Phone</span>
                    <input
                      type="text"
                      placeholder="Phone"
                      value={phone}
                      onChange={handlePhoneChange}
                    />
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
                        onChange={handleCardNumberChange}
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
                  <GBtn btnText="Save changes" clickEvent={handleSaveButton} />
                </div>
              </div>
            </div>
          ) : null}
        </div>
      </div>
      <Footer />
    </>
  );
};

export default Profile;
