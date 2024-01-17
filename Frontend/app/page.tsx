"use client";
import { Navbar } from "./components";
import "./scss/home.css";
import {
  setSessionToken,
  getSessionToken,
  removeSessionToken,
} from "../app/utils/cookie";
import { useRouter } from "next/navigation"; // Import the correct module
import { useEffect } from "react";
import { Redirect } from "next";
import { Button } from "primereact/button";

export default function Home() {
  const router = useRouter();

  if (getSessionToken() === "") {
    router.push("/login");
    return <h1>Redirecting...</h1>; // or you can render a loading state
  }
  return (
    <main>
      <Navbar idx={0} />
    </main>
  );
}
