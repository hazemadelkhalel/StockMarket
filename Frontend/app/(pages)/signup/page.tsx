"use client";
import Link from "next/link";
import { GBtn, StockerField } from "../../components";
import "./scss/signup.css";
import { useRef, useState } from "react";
import { Toast } from "primereact/toast";
import "primereact/resources/themes/lara-light-blue/theme.css";
import "primereact/resources/primereact.min.css";
import "primeicons/primeicons.css";
import { useRouter } from "next/navigation";

const SignUp = () => {
  const [username, setUsername] = useState("");
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [confirmPassword, setConfirmPassword] = useState("");
  const toast = useRef(null);
  const router = useRouter();

  const handleUsernameChange = (value: any) => {
    setUsername(value);
  };
  const handleEmailChange = (value: any) => {
    setEmail(value);
  };
  const handlePasswordChange = (value: any) => {
    setPassword(value);
  };
  const handleConfirmPasswordChange = (value: any) => {
    setConfirmPassword(value);
  };
  const handleClick = async (event: { preventDefault: () => void }) => {
    event.preventDefault();
    try {
      if (
        username === "" ||
        email === "" ||
        password === "" ||
        confirmPassword === ""
      ) {
        (toast.current as any)?.show({
          severity: "error",
          summary: "Failed",
          detail: "Missing fields",
          life: 1500,
        });
        return;
      }

      if (password !== confirmPassword) {
        (toast.current as any)?.show({
          severity: "error",
          summary: "Failed",
          detail: "Passwords do not match",
          life: 1500,
        });
        return;
      }
      const response = await fetch("http://localhost:8000/signup/create", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          User: {
            username: username,
            email: email,
            password: password,
          },
        }),
      });

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
      (toast.current as any)?.show({
        severity: "success",
        summary: "Success",
        detail: "Account created successfully",
        life: 1000,
      });
      setTimeout(() => {
        router.push("/");
      }, 1000);
    } catch (error) {
      console.error("Error sending message:", error as Error);
    }
  };

  return (
    <div className="align">
      <Toast ref={toast} />
      <div className="signup-div">
        <div className="features">
          <div className="ft-title">
            Stock<span>Market</span>
          </div>
          <div className="ft-container">
            <div className="ft-box">
              <div className="ftb-icn">
                <img src="/SVG/stockIcon.svg" alt="BadgeIcon" />
              </div>
              <div className="ftb-info">
                <h4>Stay Ahead with Live Market Updates</h4>
                <p>
                  Access real-time stock quotes, market indices, and dynamic
                  charts, empowering users with up-to-the-minute data for
                  informed investment decisions.
                </p>
              </div>
            </div>
            <div className="ft-box">
              <div className="ftb-icn">
                <img src="/SVG/analysisIcon.svg" alt="BadgeIcon" />
              </div>
              <div className="ftb-info">
                <h4>In-depth Analysis for Informed Choices</h4>
                <p>
                  Uncover market trends and insights through a rich news
                  section, providing expert commentary and analysis on financial
                  developments, ensuring users are well-equipped with the latest
                  information.
                </p>
              </div>
            </div>
            <div className="ft-box">
              <div className="ftb-icn">
                <img src="/SVG/toolsIcon.svg" alt="ShieldIcon" />
              </div>
              <div className="ftb-info">
                <h4>Effortless Portfolio Tracking and Management</h4>
                <p>
                  Seamlessly track and manage your investments with intuitive
                  portfolio tools, allowing users to monitor asset allocation,
                  analyze performance, and make strategic adjustments for a
                  well-balanced investment portfolio.
                </p>
              </div>
            </div>
          </div>
        </div>
        <div className="auth">
          <div className="auth-title">
            Sign up as a <span>Stocker</span>
          </div>
          <form className="auth-form">
            <StockerField
              giText="Username"
              giPlaceholder="Enter your username"
              giType="text"
              onInputChange={handleUsernameChange}
            />
            <StockerField
              giText="Email"
              giPlaceholder="Enter your email"
              giType="email"
              onInputChange={handleEmailChange}
            />
            <StockerField
              giText="Password"
              giPlaceholder="Enter your password"
              giType="password"
              onInputChange={handlePasswordChange}
            />
            <StockerField
              giText="Confirm Password"
              giPlaceholder="Confirm your password"
              giType="password"
              onInputChange={handleConfirmPasswordChange}
            />
            <div className="f-sbmt">
              <GBtn btnText="Create new account" clickEvent={handleClick} />
              <span className="alrg">
                Already have an account?<Link href="/login">Login</Link>
              </span>
            </div>
          </form>
        </div>
      </div>
    </div>
  );
};

export default SignUp;
