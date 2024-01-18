import "./scss/footer.css";

const Footer = () => {
  return (
    <div className="footer-container">
      <h3 className="footer-contacts">
        <img src="/SVG/facebook.svg" alt="social" />
        <img src="/SVG/twitter.svg" alt="social" />
        <img src="/SVG/youtube.svg" alt="social" />
        <img src="/SVG/discord.svg" alt="social" />
        <img src="/SVG/instagram.svg" alt="social" />
      </h3>
      <h3 className="footer-welcome">
        Empower your financial journey with Stock Market. Invest with
        confidence, trade with ease. Buy and sell stocks seamlessly on our
        advanced platform. Your gateway to a world of opportunities in the stock
        market.
      </h3>
      <h3 className="footer-copyrights">
        © 2024 Stock Market. All rights reserved. Stock Market is a registered
        trademark of Stock Market. Any unauthorized use, reproduction, or
        distribution of stock data, trading features, or informational content
        from this platform is strictly prohibited. All market insights,
        investment tools, and proprietary algorithms are protected by copyright
        law.
      </h3>
    </div>
  );
};

export default Footer;
