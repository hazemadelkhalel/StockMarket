"use client";
import Link from "next/link";
import { GBtn, StockerField } from "../../components";
import "./scss/login.css";
import { useRef, useState } from "react";
import { Toast } from "primereact/toast";
import { useRouter } from "next/navigation";
import "primereact/resources/themes/lara-light-blue/theme.css";
import "primereact/resources/primereact.min.css";
import "primeicons/primeicons.css";

const Login = () => {
  const [username_or_email, setUsernameOrEmail] = useState("");
  const [password, setPassword] = useState("");
  const toast = useRef(null);
  const router = useRouter();

  const handleUsernameOrEmailChange = (value: any) => {
    setUsernameOrEmail(value);
  };
  const handlePasswordChange = (value: any) => {
    setPassword(value);
  };

  const handleClick = async (event: { preventDefault: () => void }) => {
    event.preventDefault();
    try {
      console.log(username_or_email, password);

      if (username_or_email === "" || password === "") {
        (toast.current as any)?.show({
          severity: "error",
          summary: "Failed",
          detail: "Missing fields",
          life: 1500,
        });
        return;
      }

      const response = await fetch("http://localhost:8000/login/redirect", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          User: {
            username_or_email: username_or_email,
            password: password,
          },
        }),
      });

      const data = await response.json();
      console.log(data);
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
        detail: "Login successfully",
        life: 1000,
      });
      setTimeout(() => {
        router.push("/");
      }, 1000);
      console.log(data);
    } catch (error) {
      console.error("Error sending message:", error as Error);
    }
  };

  return (
    <div className="login-container">
      <Toast ref={toast} />
      <div className="login-div">
        <div className="auth-title">
          Login as a <span>Stocker</span>
        </div>
        <form className="auth-form">
          <StockerField
            giText="Email"
            giPlaceholder="Enter your username or email"
            giType="email"
            onInputChange={handleUsernameOrEmailChange}
          />
          <StockerField
            giText="Password"
            giPlaceholder="Enter your password"
            giType="password"
            onInputChange={handlePasswordChange}
          />
          <div className="f-sbmt">
            <GBtn btnText="Login" clickEvent={handleClick} />
            <Link href="/forgetpass" className="frg-pass">
              Forget Password?
            </Link>
            <span className="alrg">
              Don't an account?<Link href="/signup">Sign Up</Link>
            </span>
          </div>
        </form>
      </div>
    </div>
  );
};

export default Login;
