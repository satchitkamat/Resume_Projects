import React, { useState, useEffect } from 'react';
import { Link, useLocation } from 'react-router-dom';

const Navbar = () => {
    const [isScrolled, setIsScrolled] = useState(false);
    const [mobileMenuOpen, setMobileMenuOpen] = useState(false);
    const location = useLocation();

    useEffect(() => {
        const handleScroll = () => {
            setIsScrolled(window.scrollY > 50);
        };
        window.addEventListener('scroll', handleScroll);
        return () => window.removeEventListener('scroll', handleScroll);
    }, []);

    const navLinks = [
        { name: 'Home', href: '/#home' },
        { name: 'Education', href: '/#education' },
        { name: 'Projects', href: '/#projects' },
        { name: 'Contact', href: '/#contact' },
    ];

    // Helper to determine if we should use a normal anchor or a Router Link
    // If we're already on the home page, regular anchor tags handle smooth scrolling better
    // If we're on a project page, we need to route to "/" first
    const isHomePage = location.pathname === '/';

    return (
        <nav className={`fixed w-full z-50 transition-all duration-300 ${isScrolled ? 'py-4 glass shadow-lg' : 'py-6 bg-transparent'}`}>
            <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
                <div className="flex justify-between items-center">
                    <div className="flex-shrink-0">
                        <Link to="/" className="text-2xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-blue-400 to-indigo-500">
                            Portfolio
                        </Link>
                    </div>

                    {/* Desktop Menu */}
                    <div className="hidden md:block">
                        <div className="ml-10 flex items-baseline space-x-8">
                            {navLinks.map((link) => (
                                isHomePage ? (
                                    <a
                                        key={link.name}
                                        href={link.href.replace('/', '')}
                                        className="text-slate-300 hover:text-white px-3 py-2 rounded-md text-sm font-medium transition-colors"
                                    >
                                        {link.name}
                                    </a>
                                ) : (
                                    <Link
                                        key={link.name}
                                        to={link.href}
                                        className="text-slate-300 hover:text-white px-3 py-2 rounded-md text-sm font-medium transition-colors"
                                    >
                                        {link.name}
                                    </Link>
                                )
                            ))}
                            {isHomePage ? (
                                <a
                                    href="#contact"
                                    className="bg-blue-600 hover:bg-blue-700 text-white px-4 py-2 rounded-full text-sm font-medium transition-colors shadow-[0_0_15px_rgba(37,99,235,0.5)]"
                                >
                                    Let's Talk
                                </a>
                            ) : (
                                <Link
                                    to="/#contact"
                                    className="bg-blue-600 hover:bg-blue-700 text-white px-4 py-2 rounded-full text-sm font-medium transition-colors shadow-[0_0_15px_rgba(37,99,235,0.5)]"
                                >
                                    Let's Talk
                                </Link>
                            )}
                        </div>
                    </div>

                    {/* Mobile menu button */}
                    <div className="md:hidden flex items-center">
                        <button
                            onClick={() => setMobileMenuOpen(!mobileMenuOpen)}
                            className="text-slate-300 hover:text-white focus:outline-none"
                        >
                            <svg className="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                                {mobileMenuOpen ? (
                                    <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M6 18L18 6M6 6l12 12" />
                                ) : (
                                    <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M4 6h16M4 12h16M4 18h16" />
                                )}
                            </svg>
                        </button>
                    </div>
                </div>
            </div>

            {/* Mobile Menu */}
            {mobileMenuOpen && (
                <div className="md:hidden glass absolute w-full left-0 mt-2 py-4 px-4 flex flex-col space-y-4 shadow-xl border-t border-slate-700/50">
                    {navLinks.map((link) => (
                        isHomePage ? (
                            <a
                                key={link.name}
                                href={link.href.replace('/', '')}
                                onClick={() => setMobileMenuOpen(false)}
                                className="text-slate-300 hover:text-white block px-3 py-2 rounded-md text-base font-medium"
                            >
                                {link.name}
                            </a>
                        ) : (
                            <Link
                                key={link.name}
                                to={link.href}
                                onClick={() => setMobileMenuOpen(false)}
                                className="text-slate-300 hover:text-white block px-3 py-2 rounded-md text-base font-medium"
                            >
                                {link.name}
                            </Link>
                        )
                    ))}
                </div>
            )}
        </nav>
    );
};

export default Navbar;
