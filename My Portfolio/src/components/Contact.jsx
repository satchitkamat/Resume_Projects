import React from 'react';

const Contact = () => {
    return (
        <section id="contact" className="py-24 relative overflow-hidden">
            <div className="absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 w-[600px] h-[600px] rounded-full bg-indigo-500/10 blur-[120px] -z-10" />

            <div className="max-w-6xl mx-auto px-4 sm:px-6 lg:px-8">
                <div className="text-center mb-16">
                    <h2 className="text-3xl md:text-5xl font-bold text-white mb-6">
                        Ready to connect?
                    </h2>
                    <p className="text-xl text-slate-400 max-w-2xl mx-auto">
                        I'm always open to discussing new projects, creative ideas, or opportunities to be part of your visions.
                    </p>
                </div>

                <div className="grid grid-cols-1 lg:grid-cols-2 gap-12 items-start">
                    {/* Contact Information */}
                    <div className="text-left space-y-8 glass p-8 md:p-12 rounded-3xl border border-slate-700/50 shadow-2xl h-full flex flex-col justify-center">
                        <h3 className="text-3xl font-bold text-white mb-2">Get In Touch</h3>
                        <p className="text-slate-400 text-lg mb-8">
                            Feel free to reach out to me directly through email or give me a call.
                        </p>

                        <div className="space-y-8">
                            <div className="flex items-center space-x-6 group">
                                <div className="w-16 h-16 rounded-2xl glass flex items-center justify-center text-blue-400 group-hover:bg-blue-600 group-hover:text-white transition-all transform group-hover:-translate-y-1 duration-300 shadow-lg border border-slate-700">
                                    <svg className="w-8 h-8" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                                        <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M3 8l7.89 5.26a2 2 0 002.22 0L21 8M5 19h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v10a2 2 0 002 2z" />
                                    </svg>
                                </div>
                                <div>
                                    <p className="text-sm text-slate-400 font-semibold uppercase tracking-wider mb-1">Email</p>
                                    <a href="mailto:satchitkamat22@gmail.com" className="text-xl text-white hover:text-blue-400 transition-colors font-medium">satchitkamat22@gmail.com</a>
                                </div>
                            </div>

                            <div className="flex items-center space-x-6 group">
                                <div className="w-16 h-16 rounded-2xl glass flex items-center justify-center text-blue-400 group-hover:bg-blue-600 group-hover:text-white transition-all transform group-hover:-translate-y-1 duration-300 shadow-lg border border-slate-700">
                                    <svg className="w-8 h-8" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                                        <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M3 5a2 2 0 012-2h3.28a1 1 0 01.948.684l1.498 4.493a1 1 0 01-.502 1.21l-2.257 1.13a11.042 11.042 0 005.516 5.516l1.13-2.257a1 1 0 011.21-.502l4.493 1.498a1 1 0 01.684.949V19a2 2 0 01-2 2h-1C9.716 21 3 14.284 3 6V5z" />
                                    </svg>
                                </div>
                                <div>
                                    <p className="text-sm text-slate-400 font-semibold uppercase tracking-wider mb-1">Phone</p>
                                    <a href="tel:+919967866746" className="text-xl text-white hover:text-blue-400 transition-colors font-medium">+91 9967866746</a>
                                </div>
                            </div>
                        </div>
                    </div>

                    {/* Contact Form */}
                    <form className="glass p-8 md:p-12 rounded-3xl text-left space-y-6 shadow-2xl relative border border-slate-700/50">
                        <h3 className="text-2xl font-bold text-white mb-6">Send Me a Message</h3>
                        <div className="space-y-6">
                            <div>
                                <label htmlFor="name" className="block text-sm font-medium text-slate-300 mb-2">Name</label>
                                <input
                                    type="text"
                                    id="name"
                                    className="w-full bg-slate-800/80 border border-slate-700/50 rounded-xl px-5 py-4 text-slate-100 placeholder-slate-500 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent transition-all"
                                    placeholder="John Doe"
                                />
                            </div>
                            <div>
                                <label htmlFor="email" className="block text-sm font-medium text-slate-300 mb-2">Email</label>
                                <input
                                    type="email"
                                    id="email"
                                    className="w-full bg-slate-800/80 border border-slate-700/50 rounded-xl px-5 py-4 text-slate-100 placeholder-slate-500 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent transition-all"
                                    placeholder="john@example.com"
                                />
                            </div>
                        </div>
                        <div>
                            <label htmlFor="message" className="block text-sm font-medium text-slate-300 mb-2">Message</label>
                            <textarea
                                id="message"
                                rows={4}
                                className="w-full bg-slate-800/80 border border-slate-700/50 rounded-xl px-5 py-4 text-slate-100 placeholder-slate-500 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent transition-all resize-none"
                                placeholder="Message goes here..."
                            />
                        </div>
                        <button
                            type="button"
                            className="w-full py-4 rounded-xl bg-gradient-to-r from-blue-600 to-indigo-600 hover:from-blue-500 hover:to-indigo-500 text-white font-bold text-lg shadow-[0_0_30px_rgba(79,70,229,0.3)] transition-all transform hover:-translate-y-1"
                        >
                            Send Request
                        </button>
                    </form>
                </div>
            </div>
        </section>
    );
};

export default Contact;
