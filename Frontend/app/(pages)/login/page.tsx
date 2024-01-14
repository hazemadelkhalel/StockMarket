"use client";
import Link from "next/link";
import { GBtn, StockerField } from "../../components";
import "./scss/login.css";

const Login = () => {
  return (
    <div className="login-container">
      <div className="login-div">
        <div className="auth-title">
          Login as a <span>Stocker</span>
        </div>
        <form className="auth-form">
          <StockerField
            giText="Email"
            giPlaceholder="Enter your username or email"
            giType="email"
          />
          <StockerField
            giText="Password"
            giPlaceholder="Enter your password"
            giType="password"
          />
          <div className="f-sbmt">
            <GBtn btnText="Login" clickEvent={() => {}} />
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
