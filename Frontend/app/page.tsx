"use client";
import { Navbar } from "./components";
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

export default function Home() {
  const router = useRouter();
  const [username, setUsername] = useState("");

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

  return (
    <main>
      <Navbar idx={0} username={username} />
    </main>
  );
}
