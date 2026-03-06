import React from 'react';

const Education = () => {
    return (
        <section id="education" className="py-24 relative">
            <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
                <div className="text-center mb-16">
                    <h2 className="text-3xl md:text-5xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-blue-400 to-indigo-500 mb-6">
                        Education
                    </h2>
                    <p className="text-slate-400 text-lg max-w-2xl mx-auto">
                        My academic journey and qualifications.
                    </p>
                </div>

                <div className="space-y-6 max-w-4xl mx-auto">
                    {/* BCA */}
                    <div className="glass p-8 rounded-2xl border border-slate-700/50 hover:border-blue-500/50 transition-colors relative overflow-hidden group shadow-lg">
                        <div className="absolute top-0 left-0 w-2 h-full bg-blue-500 transform origin-left transition-transform group-hover:scale-x-150"></div>
                        <div className="flex flex-col md:flex-row justify-between items-start md:items-center pl-4">
                            <div>
                                <h3 className="text-2xl font-bold text-white mb-2">Bachelor of Computer Applications (BCA)</h3>
                                <p className="text-lg text-slate-400">First Year Student</p>
                            </div>
                            <div className="mt-4 md:mt-0">
                                <span className="px-5 py-2 rounded-full bg-blue-500/10 text-blue-400 font-semibold border border-blue-500/20">
                                    Currently Pursuing
                                </span>
                            </div>
                        </div>
                    </div>

                    {/* 12th */}
                    <div className="glass p-8 rounded-2xl border border-slate-700/50 hover:border-indigo-500/50 transition-colors relative overflow-hidden group shadow-lg">
                        <div className="absolute top-0 left-0 w-2 h-full bg-indigo-500 transform origin-left transition-transform group-hover:scale-x-150"></div>
                        <div className="flex flex-col md:flex-row justify-between items-start md:items-center pl-4">
                            <div>
                                <h3 className="text-2xl font-bold text-white mb-2">12th Grade (HSC)</h3>
                                <p className="text-lg text-slate-400">Passed with 79%</p>
                            </div>
                            <div className="mt-4 md:mt-0">
                                <span className="px-5 py-2 rounded-full bg-slate-800 text-slate-300 font-semibold border border-slate-700">
                                    2024 - 2025
                                </span>
                            </div>
                        </div>
                    </div>

                    {/* 10th */}
                    <div className="glass p-8 rounded-2xl border border-slate-700/50 hover:border-purple-500/50 transition-colors relative overflow-hidden group shadow-lg">
                        <div className="absolute top-0 left-0 w-2 h-full bg-purple-500 transform origin-left transition-transform group-hover:scale-x-150"></div>
                        <div className="flex flex-col md:flex-row justify-between items-start md:items-center pl-4">
                            <div>
                                <h3 className="text-2xl font-bold text-white mb-2">10th Grade (SSC)</h3>
                                <p className="text-lg text-slate-400">Passed with 69%</p>
                            </div>
                            <div className="mt-4 md:mt-0">
                                <span className="px-5 py-2 rounded-full bg-slate-800 text-slate-300 font-semibold border border-slate-700">
                                    2022 - 2023
                                </span>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </section>
    );
};

export default Education;
