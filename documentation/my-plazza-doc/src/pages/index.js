import React from 'react';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';

export default function Home() {
  return (
    <Layout
      title="Welcome to the Plazza Documentation"
      description="Explore the documentation for the Plazza project">
      <main style={{ textAlign: 'center', padding: '2rem' }}>
        <h1>Welcome to the Plazza Documentation</h1>
        <p>Explore the different sections of the documentation:</p>
        <div style={{ marginTop: '2rem' }}>
          <Link to="/docs/Reception" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🛎️ Reception
          </Link>
          <Link to="/docs/kitchen" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🔪 Kitchen
          </Link>
          <Link to="/docs/cook" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🧑‍🍳 Cook
          </Link>
          <Link to="/docs/errors" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            ❗ Errors
          </Link>
          <Link to="/docs/receipy" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🍽️ Receipy
          </Link>
          <Link to="/docs/process" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            ⚙️ Process
          </Link>
          <Link to="/docs/loader" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            📖 Dlloader
          </Link>
          <Link to="/docs/icp" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            📑 ICP
          </Link>
          <Link to="/docs/archi" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🏗️ Architecture
          </Link>
        </div>
        <div style={{ marginTop: '3rem' }}>
          <a
            href="/DocPlazza.pdf"
            target="_blank"
            rel="noopener noreferrer"
            style={{ fontSize: '1.1rem', color: '#0078e7', textDecoration: 'underline' }}
          >
            📚 View Full Doxygen PDF Documentation
          </a>
        </div>
      </main>
    </Layout>
  );
}
