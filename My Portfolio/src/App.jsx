import React from 'react';
import Navbar from './components/Navbar';
import Hero from './components/Hero';
import Education from './components/Education';
import Projects from './components/Projects';
import Contact from './components/Contact';
import Footer from './components/Footer';

import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import ProjectDetails from './components/ProjectDetails';
import ScrollToAnchor from './components/ScrollToAnchor';

const Home = () => (
  <>
    <Hero />
    <Education />
    <Projects />
    <Contact />
  </>
);

function App() {
  return (
    <Router>
      <ScrollToAnchor />
      <div className="min-h-screen bg-slate-900 text-slate-50 font-sans selection:bg-blue-500 selection:text-white">
        <Navbar />
        <main>
          <Routes>
            <Route path="/" element={<Home />} />
            <Route path="/project/:id" element={<ProjectDetails />} />
          </Routes>
        </main>
        <Footer />
      </div>
    </Router>
  );
}

export default App;
