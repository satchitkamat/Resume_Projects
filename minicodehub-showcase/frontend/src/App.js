import React, { useState, useEffect, useCallback, useRef } from 'react';
import { BrowserRouter as Router, Routes, Route, Link, useParams, useNavigate } from 'react-router-dom';
import { Play, Code, Monitor, ChevronLeft, Square, Search, Filter } from 'lucide-react';
import io from 'socket.io-client';

// Navbar (unchanged - perfect)
function Navbar({ filter, setFilter, search, setSearch, stats, scrollDirection, isScrolled }) {
  return (
    <header className={`
      fixed top-0 left-0 right-0 z-50 transition-all duration-500 ease-out backdrop-blur-xl
      ${isScrolled
        ? 'py-2 shadow-2xl bg-black/90 border-b border-purple-500/70 h-16'
        : 'py-4 bg-black/50 h-20'
      }
      ${scrollDirection === 'down' && isScrolled ? '-translate-y-full' : 'translate-y-0'}
    `}>
      <div className="max-w-7xl mx-auto px-4 h-full flex items-center justify-between">
        <div className={`transition-all duration-300 ${isScrolled ? 'scale-90 opacity-90' : 'scale-100 opacity-100'}`}>
          <h1 className={`font-black bg-gradient-to-r from-purple-400 via-pink-400 to-purple-500 bg-clip-text text-transparent drop-shadow-2xl transition-all ${isScrolled ? 'text-xl leading-tight' : 'text-3xl md:text-4xl leading-tight'
            }`}>
            MiniCodeHub
          </h1>
          <p className={`text-purple-300 text-xs transition-all ${isScrolled ? 'opacity-0 scale-75 -translate-y-1' : 'opacity-100 scale-100'} hidden lg:block`}>
            Live Code Demos
          </p>
        </div>

        <div className="flex items-center gap-3 flex-1 max-w-md justify-center">
          <div className="relative flex-1">
            <svg className={`absolute left-3 top-1/2 -translate-y-1/2 transition-all w-${isScrolled ? '4' : '5'} h-${isScrolled ? '4' : '5'} text-gray-400`} fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M21 21l-6-6m2-5a7 7 0 11-14 0 7 7 0 0114 0z" />
            </svg>
            <input
              type="text"
              placeholder={isScrolled ? "Search..." : "Search HTML, React, C++..."}
              className={`w-full pl-${isScrolled ? '10' : '12'} pr-4 py-${isScrolled ? '1.5' : '3'} bg-white/10 backdrop-blur-md border border-white/20 rounded-xl focus:outline-none focus:ring-2 focus:ring-purple-500/50 transition-all text-${isScrolled ? 'base' : 'lg'} shadow-lg`}
              value={search}
              onChange={(e) => setSearch(e.target.value)}
            />
          </div>
        </div>

        <div className={`flex bg-white/10 backdrop-blur-xl rounded-2xl p-${isScrolled ? '1' : '2'} border border-white/30 shadow-xl transition-all overflow-hidden ${isScrolled ? 'w-80 h-12' : 'w-96 h-16'}`}>
          <button onClick={() => setFilter('all')} className={`px-${isScrolled ? '3' : '5'} py-${isScrolled ? '1' : '3'} rounded-xl font-bold text-xs transition-all flex items-center gap-1 shadow-sm ${filter === 'all' ? 'bg-gradient-to-r from-purple-500 to-pink-500 text-white shadow-purple-500/50' : 'text-purple-200 hover:text-white hover:bg-white/30 hover:shadow-md'}`}>
            ALL <span className="text-xs bg-black/50 px-1 py-0.5 rounded min-w-[2rem] font-bold">{stats.total}</span>
          </button>
          <button onClick={() => setFilter('html')} className={`px-${isScrolled ? '2' : '4'} py-${isScrolled ? '1' : '3'} rounded-xl font-bold text-xs flex items-center gap-1 transition-all ${filter === 'html' ? 'bg-gradient-to-r from-orange-500 to-orange-600 text-white shadow-orange-500/50' : 'text-orange-300 hover:text-orange-100 hover:bg-orange-500/20 hover:shadow-md'}`}>
            HTML <span className="text-xs bg-black/50 px-1 py-0.5 rounded min-w-[1.5rem] font-bold">{stats.html}</span>
          </button>
          <button onClick={() => setFilter('reactjs')} className={`px-${isScrolled ? '2' : '4'} py-${isScrolled ? '1' : '3'} rounded-xl font-bold text-xs flex items-center gap-1 transition-all ${filter === 'reactjs' ? 'bg-gradient-to-r from-emerald-500 to-teal-600 text-white shadow-emerald-500/50' : 'text-emerald-300 hover:text-emerald-100 hover:bg-emerald-500/20 hover:shadow-md'}`}>
            React <span className="text-xs bg-black/50 px-1 py-0.5 rounded min-w-[1.5rem] font-bold">{stats.react}</span>
          </button>
          <button onClick={() => setFilter('cpp')} className={`px-${isScrolled ? '2' : '4'} py-${isScrolled ? '1' : '3'} rounded-xl font-bold text-xs flex items-center gap-1 transition-all ${filter === 'cpp' ? 'bg-gradient-to-r from-blue-500 to-indigo-600 text-white shadow-blue-500/50' : 'text-blue-300 hover:text-blue-100 hover:bg-blue-500/20 hover:shadow-md'}`}>
            C++ <span className="text-xs bg-black/50 px-1 py-0.5 rounded min-w-[1.5rem] font-bold">{stats.cpp}</span>
          </button>
        </div>
      </div>
    </header>
  );
}

function Home() {
  const [videos, setVideos] = useState([]);
  const [search, setSearch] = useState('');
  const [filter, setFilter] = useState('all');
  const [loading, setLoading] = useState(true);
  const [stats, setStats] = useState({ total: 0, html: 0, react: 0, cpp: 0 });
  const [currentPage, setCurrentPage] = useState(1);
  const [totalPages, setTotalPages] = useState(1);
  const [scrollY, setScrollY] = useState(0);
  const [lastScrollY, setLastScrollY] = useState(0);
  const [scrollDirection, setScrollDirection] = useState('up');
  const [isScrolled, setIsScrolled] = useState(false);

  // ✅ FIXED: useCallback WITHOUT currentPage dependency
  const fetchVideos = useCallback(async () => {
    setLoading(true);
    try {
      const params = new URLSearchParams({
        search,
        lang: filter,
        page: currentPage,
        limit: 20
      });

      console.log('🔄 Fetching:', params.toString()); // DEBUG

      const [videosRes, statsRes] = await Promise.all([
        fetch(`/api/videos?${params}`),
        fetch('/api/stats')
      ]);

      if (!videosRes.ok || !statsRes.ok) {
        throw new Error(`Server returned ${videosRes.status} ${statsRes.status}`);
      }

      const videoContentType = videosRes.headers.get("content-type");
      if (!videoContentType || !videoContentType.includes("application/json")) {
        throw new Error("Received non-JSON response from server");
      }

      const videoData = await videosRes.json();
      const statsData = await statsRes.json();

      console.log('📊 Page data:', videoData); // DEBUG

      setVideos(Array.isArray(videoData.videos) ? videoData.videos : []);
      setTotalPages(videoData.totalPages || 1);
      setStats({
        total: statsData.total || 0,
        html: statsData.html || 0,
        react: statsData.react || 0,
        cpp: statsData.cpp || 0
      });
    } catch (error) {
      console.error('❌ API Error:', error);
      setVideos([]);
    } finally {
      setLoading(false);
    }
  }, [search, filter, currentPage]); // ✅ FIXED: Included currentPage!

  // ✅ FIXED SCROLL HANDLER
  useEffect(() => {
    let ticking = false;
    const updateScroll = () => {
      const newScrollY = window.scrollY;
      setScrollY(newScrollY);
      setIsScrolled(newScrollY > 50);

      if (!ticking) {
        requestAnimationFrame(() => {
          setScrollDirection(newScrollY > lastScrollY ? 'down' : 'up');
          setLastScrollY(newScrollY);
          ticking = false;
        });
        ticking = true;
      }
    };

    window.addEventListener('scroll', updateScroll, { passive: true });
    return () => window.removeEventListener('scroll', updateScroll);
  }, [lastScrollY]);

  // ✅ FIXED: SEPARATE useEffect for pagination
  useEffect(() => {
    console.log('📄 Page changed to:', currentPage, 'Filter:', filter); // DEBUG
    fetchVideos();
  }, [currentPage, search, filter]); // ✅ Now works perfectly!

  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-900 via-purple-900/30 to-slate-900 text-white">
      <Navbar
        filter={filter}
        setFilter={setFilter}
        search={search}
        setSearch={setSearch}
        stats={stats}
        scrollDirection={scrollDirection}
        isScrolled={isScrolled}
      />

      <main className="pt-24 lg:pt-28 pb-12">
        {/* Filter Status */}
        <div className="max-w-7xl mx-auto px-6 mb-16 pt-8">
          <div className={`inline-flex items-center gap-4 px-8 py-5 rounded-3xl backdrop-blur-xl font-bold text-xl bg-white/10 border border-white/20 shadow-2xl ${filter !== 'all' ? 'border-purple-500/50 bg-purple-500/10 ring-2 ring-purple-500/30' : ''
            }`}>
            <div className={`w-6 h-6 rounded-full shadow-lg ${filter === 'html' ? 'bg-orange-400' :
              filter === 'reactjs' ? 'bg-emerald-400' :
                filter === 'cpp' ? 'bg-blue-400' : 'bg-purple-400'
              }`} />
            <span>{filter === 'all' ? 'ALL' : filter.toUpperCase()} TUTORIALS</span>
            <span className="text-purple-300">({videos.length})</span>
          </div>
        </div>

        <div className="max-w-7xl mx-auto px-6">
          {loading ? (
            <div className="flex flex-col items-center py-40">
              <div className="w-36 h-36 border-8 border-purple-500/20 border-t-purple-500 rounded-full animate-spin mb-16 shadow-2xl"></div>
              <p className="text-4xl font-bold text-purple-300 tracking-wide">Loading {filter.toUpperCase()}...</p>
            </div>
          ) : videos.length === 0 ? (
            <div className="text-center py-40 max-w-2xl mx-auto">
              <div className="w-36 h-36 bg-white/10 backdrop-blur-xl rounded-3xl p-10 mx-auto mb-12 flex items-center justify-center shadow-2xl border border-white/20">
                <Monitor className="w-24 h-24 text-gray-500" />
              </div>
              <h2 className="text-5xl font-black text-gray-300 mb-8">No {filter} tutorials</h2>
              <p className="text-2xl text-gray-400 leading-relaxed">Try switching filters or search terms</p>
            </div>
          ) : (
            <>
              <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4 2xl:grid-cols-5 gap-8 mb-16">
                {videos.map((video) => (
                  <Link key={video.id} to={`/video/${video.id}`} className="group block h-full">
                    <div className="bg-white/10 backdrop-blur-md rounded-3xl overflow-hidden border border-white/20 hover:border-purple-500/50 hover:bg-white/20 transition-all duration-500 hover:-translate-y-3 hover:shadow-2xl h-full flex flex-col">
                      <div className="relative h-48 overflow-hidden bg-gray-800/50">
                        <img
                          src={video.thumbnail.replace('mqdefault.jpg', 'hqdefault.jpg')}
                          alt={video.title}
                          className="w-full h-full object-cover group-hover:scale-110 transition-transform duration-700"
                          loading="lazy"
                        />
                        <span className={`absolute top-4 left-4 px-3 py-1 rounded-full text-xs font-black uppercase tracking-wider shadow-lg ${video.codeLang === 'html' ? 'bg-orange-500/95 text-white' :
                          video.codeLang === 'reactjs' ? 'bg-emerald-500/95 text-white' :
                            video.codeLang === 'cpp' ? 'bg-blue-500/95 text-white' :
                              'bg-purple-500/95 text-white'
                          }`}>
                          {video.codeLang?.toUpperCase()}
                        </span>
                      </div>
                      <div className="p-6 flex-1 flex flex-col justify-between">
                        <h3 className="font-bold text-lg mb-4 line-clamp-2 group-hover:text-purple-300 transition-colors leading-tight">
                          {video.title}
                        </h3>
                        <div className="inline-flex items-center gap-2 bg-gradient-to-r from-purple-600/90 to-pink-600/90 backdrop-blur-sm text-white font-semibold px-6 py-3 rounded-2xl transition-all group-hover:from-purple-700 group-hover:to-pink-700 w-full justify-center">
                          <Play className="w-5 h-5" />
                          Watch + Code
                        </div>
                      </div>
                    </div>
                  </Link>
                ))}
              </div>

              {/* ✅ FIXED PAGINATION BUTTONS */}
              {totalPages > 1 && (
                <div className="flex flex-col sm:flex-row items-center justify-center gap-6 bg-white/5 backdrop-blur-xl rounded-3xl p-8 border border-white/20 shadow-2xl">
                  <div className="text-purple-300 font-bold text-xl">
                    Page {currentPage} of {totalPages} • {videos.length} tutorials
                  </div>

                  <div className="flex items-center gap-3">
                    <button
                      onClick={() => setCurrentPage(p => Math.max(1, p - 1))}
                      disabled={currentPage === 1 || loading}
                      className={`px-6 py-3 rounded-2xl font-bold text-lg transition-all shadow-lg flex items-center gap-2 ${currentPage === 1 || loading
                        ? 'bg-gray-700/50 text-gray-400 cursor-not-allowed'
                        : 'bg-gradient-to-r from-purple-500 to-pink-500 hover:from-purple-600 hover:to-pink-600 text-white shadow-purple-500/50 hover:shadow-xl'
                        }`}
                    >
                      <ChevronLeft className="w-5 h-5" />
                      Previous
                    </button>

                    {Array.from({ length: Math.min(7, totalPages) }, (_, i) => {
                      const pageNum = currentPage <= 4
                        ? i + 1
                        : currentPage >= totalPages - 3
                          ? totalPages - 6 + i
                          : currentPage - 3 + i;

                      if (pageNum < 1 || pageNum > totalPages) return null;

                      return (
                        <button
                          key={pageNum}
                          onClick={() => setCurrentPage(pageNum)}
                          disabled={loading}
                          className={`w-14 h-14 rounded-2xl font-bold text-lg transition-all shadow-lg flex items-center justify-center ${currentPage === pageNum
                            ? 'bg-gradient-to-r from-purple-500 to-pink-500 text-white shadow-purple-500/50 scale-110 ring-4 ring-purple-500/30'
                            : loading
                              ? 'bg-gray-700/50 text-gray-400 cursor-not-allowed'
                              : 'bg-white/10 hover:bg-white/20 text-purple-200 hover:text-white hover:shadow-xl hover:scale-105'
                            }`}
                        >
                          {pageNum}
                        </button>
                      );
                    })}

                    <button
                      onClick={() => setCurrentPage(p => Math.min(totalPages, p + 1))}
                      disabled={currentPage === totalPages || loading}
                      className={`px-6 py-3 rounded-2xl font-bold text-lg transition-all shadow-lg flex items-center gap-2 ${currentPage === totalPages || loading
                        ? 'bg-gray-700/50 text-gray-400 cursor-not-allowed'
                        : 'bg-gradient-to-r from-purple-500 to-pink-500 hover:from-purple-600 hover:to-pink-600 text-white shadow-purple-500/50 hover:shadow-xl'
                        }`}
                    >
                      Next
                      <ChevronLeft className="w-5 h-5 rotate-180" />
                    </button>
                  </div>
                </div>
              )}
            </>
          )}
        </div>
      </main>
    </div>
  );
}

// VideoDetail with Socket.IO
function VideoDetail() {
  const { id } = useParams();
  const [video, setVideo] = useState(null);
  const navigate = useNavigate();
  const socketRef = useRef(null);
  const terminalEndRef = useRef(null);
  const [visualLogs, setVisualLogs] = useState([]); // For React/HTML console logs
  const [srcDoc, setSrcDoc] = useState('');

  // Initialize Socket (Keep for C++)
  useEffect(() => {
    socketRef.current = io('http://localhost:3001');
    return () => {
      if (socketRef.current) socketRef.current.disconnect();
    };
  }, []);

  // Socket Events (C++)
  useEffect(() => {
    if (!socketRef.current) return;

    socketRef.current.on('output', (data) => {
      setVideo(v => {
        if (!v) return null;
        return {
          ...v,
          terminalOutput: (v.terminalOutput || '') + data
        };
      });
      // Auto-scroll
      if (terminalEndRef.current) {
        terminalEndRef.current.scrollIntoView({ behavior: 'smooth' });
      }
    });

    socketRef.current.on('done', () => {
      setVideo(v => v ? ({ ...v, isRunning: false }) : null);
    });

    return () => {
      socketRef.current.off('output');
      socketRef.current.off('done');
    };
  }, []);

  // Listen for iframe messages (React Logs)
  useEffect(() => {
    const handleMessage = (event) => {
      if (event.data?.type === 'console') {
        setVisualLogs(prev => [...prev, {
          level: event.data.level,
          args: event.data.args,
          timestamp: new Date().toLocaleTimeString()
        }]);
        if (terminalEndRef.current) {
          terminalEndRef.current.scrollIntoView({ behavior: 'smooth' });
        }
      }
    };
    window.addEventListener('message', handleMessage);
    return () => window.removeEventListener('message', handleMessage);
  }, []);

  // Fetch Video
  useEffect(() => {
    fetch(`/api/video/${id}`)
      .then(res => {
        if (!res.ok) throw new Error(`Server returned ${res.status}`);
        return res.json();
      })
      .then(data => {
        setVideo({ ...data, terminalOutput: '', isRunning: false });
        // Set visual logs to empty
        setVisualLogs([]);
        // Generate srcDoc
        generateSrcDoc(data);
      })
      .catch((err) => {
        console.error("Video Fetch Error:", err);
      });
  }, [id]);

  const generateSrcDoc = (videoData) => {
    if (!videoData?.codeData) return;
    const code = videoData.codeData.fetchedCode || videoData.codeData.code;

    console.log('🎨 Generating SrcDoc for:', videoData.codeLang); // DEBUG

    if (videoData.codeLang === 'reactjs') {
      // Preprocess: Remove imports/exports for Babel Standalone

      // 1. Parsing and Mocking/Restoring Imports
      let injectedCode = '';

      // Regex to capture the header (stuff between import and from) and the library
      const importAllRegex = /import\s+([\s\S]*?)\s+from\s+['"]([^'"]+)['"];?/g;

      let match;
      while ((match = importAllRegex.exec(code)) !== null) {
        const rawImports = match[1];
        const libraryName = match[2];

        let defaultImport = null;
        let namedImports = [];

        // Detect Named Imports: { ... }
        const namedMatch = rawImports.match(/\{([^}]+)\}/);
        if (namedMatch) {
          const insideBraces = namedMatch[1];
          namedImports = insideBraces.split(',').map(i => i.trim()).filter(i => i);
        }

        // Detect Default Import: Everything outside { ... }
        let outsideBraces = rawImports.replace(/\{[^}]+\}/g, '').replace(/,/g, '').trim();
        if (outsideBraces) {
          // Handle "import * as X" vs "import X"
          if (outsideBraces.startsWith('* as ')) {
            defaultImport = outsideBraces.replace('* as ', '').trim();
          } else {
            defaultImport = outsideBraces;
          }
        }

        // --- GENERATE INJECTIONS ---

        if (libraryName === 'react') {
          // React is global, so 'import React' is handled.
          // But 'import { useState }' needs restoration.
          if (namedImports.length > 0) {
            // Destructure from global React
            // Handle "X as Y": import { useState as us } ... -> const { useState: us } = React;
            const destructuring = namedImports.map(i => {
              if (i.includes(' as ')) return i.replace(' as ', ': ');
              return i;
            }).join(', ');

            injectedCode += `const { ${destructuring} } = React;\n`;
          }
        } else if (libraryName === 'react-dom') {
          // Similarly for ReactDOM
          if (namedImports.length > 0) {
            injectedCode += `const { ${namedImports.join(', ')} } = ReactDOM;\n`;
          }
        } else {
          // EXTERNAL LIBRARIES -> MOCK THEM
          console.log(`📦 Mocking ${libraryName}: Default=${defaultImport}, Named=[${namedImports}]`);

          const mockComponent = (name) => `const ${name} = (props) => <div style={{display:'inline-flex', alignItems:'center', justifyContent:'center', border:'1px dashed #ccc', padding:'4px', borderRadius:'4px', color: '#888', background:'#f5f5f5', fontSize:'10px' }}>📦 ${name}</div>;`;

          if (defaultImport) {
            injectedCode += mockComponent(defaultImport) + '\n';
          }

          namedImports.forEach(item => {
            const name = item.split(' as ')[1] || item;
            injectedCode += mockComponent(name) + '\n';
          });
        }
      }

      // 2. Remove all imports from the code
      let cleanCode = code.replace(/import\s+[\s\S]*?from\s+['"].*?['"];?/g, '');

      // 3. Inject our generated code at the top
      cleanCode = injectedCode + '\n' + cleanCode;

      let componentName = 'App';

      // Handle export default function Name() {}
      const defaultFuncMatch = cleanCode.match(/export\s+default\s+function\s+(\w+)/);
      if (defaultFuncMatch) {
        componentName = defaultFuncMatch[1];
        cleanCode = cleanCode.replace(/export\s+default\s+function/g, 'function');
      }

      // Handle export default class Name {}
      const defaultClassMatch = cleanCode.match(/export\s+default\s+class\s+(\w+)/);
      if (defaultClassMatch) {
        componentName = defaultClassMatch[1];
        cleanCode = cleanCode.replace(/export\s+default\s+class/g, 'class');
      }

      // Handle export default Name;
      const defaultExportMatch = cleanCode.match(/export\s+default\s+(\w+);?/);
      if (defaultExportMatch) {
        componentName = defaultExportMatch[1];
        cleanCode = cleanCode.replace(/export\s+default\s+\w+;?/g, '');
      }

      // If we found a name that isn't App, alias it!
      if (componentName !== 'App') {
        cleanCode += `
          // ID: Component Alias
          const App = ${componentName};
        `;
      }

      console.log('🧹 Cleaned Code:', cleanCode.slice(0, 100) + '...'); // DEBUG

      const doc = `
        <!DOCTYPE html>
        <html>
          <head>
            <meta charset="UTF-8" />
            <script src="https://unpkg.com/react@18/umd/react.development.js" crossorigin></script>
            <script src="https://unpkg.com/react-dom@18/umd/react-dom.development.js" crossorigin></script>
            <script src="https://unpkg.com/@babel/standalone/babel.min.js"></script>
            <link href="https://cdn.jsdelivr.net/npm/tailwindcss@2.2.19/dist/tailwind.min.css" rel="stylesheet">
            <style>
              body { background: white; color: #1a1a1a; padding: 20px; font-family: sans-serif; }
              #root { height: 100%; }
            </style>
          </head>
          <body>
            <div id="root"></div>
            <script>
              // Console Proxy
              const consoleProxy = (level, ...args) => {
                // Convert args to strings for safe transport
                const safeArgs = args.map(arg => {
                  try {
                    return typeof arg === 'object' ? JSON.stringify(arg) : String(arg);
                  } catch (e) { return String(arg); }
                });
                window.parent.postMessage({ type: 'console', level, args: safeArgs }, '*');
              };
              console.log = (...args) => consoleProxy('log', ...args);
              console.error = (...args) => consoleProxy('error', ...args);
              console.warn = (...args) => consoleProxy('warn', ...args);
              window.onerror = (msg) => consoleProxy('error', msg);
            </script>
            <script type="text/babel" data-presets="env,react,typescript">
              ${cleanCode}
              
              // Attempt to render App
              try {
                const root = ReactDOM.createRoot(document.getElementById('root'));
                if (typeof App !== 'undefined') {
                  root.render(<App />);
                  console.log("✅ React App Mounted successfully!");
                } else {
                  // If App is not defined, check if we captured a default export name
                  // This part is handled by the regex logic above injecting 'const App = Name'
                  // If that failed, look for any capitalized function
                  const globals = Object.keys(window);
                  const candidate = globals.find(key => 
                     typeof window[key] === 'function' && /^[A-Z]/.test(key) && 
                     !['React', 'ReactDOM', 'Babel'].includes(key)
                  );
                  
                  if (candidate) {
                      console.log("⚠️ App not found, trying to render detected component:", candidate);
                      const Component = window[candidate];
                      root.render(<Component />);
                  } else {
                      console.error("Could not find component 'App' or any suitable candidate.");
                  }
                }
              } catch (err) {
                console.error("Render Error: " + err.message);
              }
            </script>
          </body>
        </html>
      `;
      setSrcDoc(doc);
    } else if (videoData.codeLang === 'html') {
      setSrcDoc(code);
    }
  };

  const handleRun = () => {
    if (!socketRef.current || !video) return;
    setVideo(v => ({ ...v, isRunning: true, terminalOutput: '' }));
    socketRef.current.emit('run-cpp', {
      code: video.codeData?.fetchedCode || video.codeData?.code
    });
  };

  const handleStop = () => {
    setVideo(v => ({ ...v, isRunning: false }));
    if (socketRef.current) {
      socketRef.current.disconnect();
      socketRef.current.connect();
    }
  };

  const handleTerminalInput = (e) => {
    if (e.key === 'Enter') {
      const input = e.target.value;
      if (socketRef.current && video.isRunning) {
        socketRef.current.emit('input', input);
        setVideo(v => ({
          ...v,
          terminalOutput: (v.terminalOutput || '') + input + '\n',
          inputBuffer: ''
        }));
      }
    }
  };

  if (!video) {
    return (
      <div className="min-h-screen flex items-center justify-center bg-gradient-to-br from-slate-900 to-purple-900 pt-40">
        <div className="animate-spin rounded-full h-32 w-32 border-b-2 border-purple-500"></div>
      </div>
    );
  }

  // Layout Helpers
  const isHtml = video.codeLang === 'html';
  const isReact = video.codeLang === 'reactjs';
  const isCpp = video.codeLang === 'cpp' || video.codeLang === 'c++';

  return (
    <div className="min-h-screen pt-2 pb-12 px-6 bg-gradient-to-br from-slate-900 to-purple-900">
      <button
        onClick={() => navigate(-1)}
        className="mb-8 inline-flex items-center gap-3 text-purple-400 hover:text-white font-bold text-xl pb-2 border-b-2 border-transparent hover:border-purple-400 transition-all duration-300"
      >
        <ChevronLeft size={24} />
        Back
      </button>

      {/* Top Section: Info & Video */}
      <div className="max-w-[1920px] mx-auto mb-8 flex flex-col items-center">
        {/* Info */}
        <div className="w-full flex justify-between items-end mb-6">
          <div className="shrink-0">
            <h1 className="text-3xl font-black text-transparent bg-clip-text bg-gradient-to-r from-white to-gray-400 mb-2 truncate">
              {video.title}
            </h1>
            <div className="flex gap-4 items-center">
              <span className={`px-3 py-1 rounded-lg text-xs font-bold uppercase tracking-wider ${isHtml ? 'bg-orange-500/20 text-orange-300' :
                isReact ? 'bg-emerald-500/20 text-emerald-300' :
                  'bg-blue-500/20 text-blue-300'
                }`}>
                {video.codeLang}
              </span>
            </div>
          </div>
        </div>

        {/* Video Player - Centered & Large */}
        <div className="relative w-full max-w-5xl aspect-video bg-black rounded-3xl overflow-hidden shadow-2xl shrink-0">
          <iframe
            src={`https://www.youtube.com/embed/${video.videoId}?rel=0`}
            className="w-full h-full"
            allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
            allowFullScreen
          />
        </div>
      </div>

      {/* Bottom Section: Code & Preview Side-by-Side */}
      <div className="max-w-[1920px] mx-auto grid grid-cols-1 lg:grid-cols-2 gap-8 items-start">
        {/* Left Column: Code View */}
        <div className="h-[600px] bg-[#1e1e1e] rounded-3xl overflow-hidden border border-white/10 shadow-xl flex flex-col">
          <div className="flex items-center justify-between px-6 py-3 bg-white/5">
            <div className="flex items-center gap-2">
              <Code className="w-4 h-4 text-purple-400" />
              <span className="font-bold text-sm text-gray-300">Source Code</span>
            </div>
          </div>
          <div className="flex-1 overflow-auto p-4 font-mono text-xs leading-relaxed custom-scrollbar text-gray-300">
            <pre>{video.codeData?.fetchedCode || video.codeData?.code}</pre>
          </div>
        </div>

        {/* Right Column: Execution / Preview */}
        <div className="h-[600px] flex flex-col gap-6">
          {/* If HTML: Full Preview */}
          {isHtml && (
            <div className="h-full bg-white rounded-3xl overflow-hidden shadow-2xl border border-white/20 flex flex-col">
              <div className="bg-gray-100 px-4 py-2 border-b border-gray-200 flex items-center justify-between">
                <span className="text-xs font-bold text-gray-500 uppercase tracking-wider">Browser Preview</span>
                <div className="flex gap-1.5">
                  <div className="w-2.5 h-2.5 rounded-full bg-red-400"></div>
                  <div className="w-2.5 h-2.5 rounded-full bg-yellow-400"></div>
                  <div className="w-2.5 h-2.5 rounded-full bg-green-400"></div>
                </div>
              </div>
              <iframe srcDoc={srcDoc} title="preview" className="flex-1 w-full border-none bg-white" sandbox="allow-scripts" />
            </div>
          )}

          {/* If React: Full Preview (No Terminal) */}
          {isReact && (
            <div className="h-full bg-white rounded-3xl overflow-hidden shadow-2xl border border-white/20 flex flex-col">
              <div className="bg-gray-100 px-4 py-2 border-b border-gray-200 flex items-center justify-between shrink-0">
                <span className="text-xs font-bold text-gray-500 uppercase tracking-wider">React Live Preview</span>
                <div className="flex gap-1.5">
                  <div className="w-2.5 h-2.5 rounded-full bg-red-400"></div>
                  <div className="w-2.5 h-2.5 rounded-full bg-yellow-400"></div>
                  <div className="w-2.5 h-2.5 rounded-full bg-green-400"></div>
                </div>
              </div>
              <iframe srcDoc={srcDoc} title="preview" className="flex-1 w-full border-none bg-white" sandbox="allow-scripts" />
            </div>
          )}

          {/* If C++: Full Terminal */}
          {isCpp && (
            <div className="h-full bg-[#1e1e1e] rounded-3xl overflow-hidden shadow-2xl border border-white/10 flex flex-col">
              <div className="flex items-center justify-between px-6 py-4 bg-[#2d2d2d] border-b border-black/20 shrink-0">
                <div className="flex items-center gap-3">
                  <div className="flex gap-2">
                    <div className="w-3 h-3 rounded-full bg-red-500/80"></div>
                    <div className="w-3 h-3 rounded-full bg-yellow-500/80"></div>
                    <div className="w-3 h-3 rounded-full bg-green-500/80"></div>
                  </div>
                  <span className="font-mono text-sm text-gray-400 ml-2">minicodehub-terminal</span>
                </div>
                <div className="flex gap-2">
                  <button
                    onClick={handleRun}
                    disabled={video.isRunning}
                    className={`px-4 py-1.5 rounded-lg text-xs font-bold uppercase tracking-wider transition-all flex items-center gap-2 ${video.isRunning
                      ? 'bg-gray-700 text-gray-500 cursor-wait'
                      : 'bg-green-600 hover:bg-green-500 text-white shadow-lg transform hover:scale-105'
                      }`}
                  >
                    <Play size={12} className={video.isRunning ? 'animate-spin' : ''} />
                    {video.isRunning ? 'Compiling...' : 'Run Code'}
                  </button>
                  {video.isRunning && (
                    <button
                      onClick={handleStop}
                      className="px-4 py-1.5 rounded-lg text-xs font-bold uppercase tracking-wider bg-red-600 hover:bg-red-500 text-white shadow-lg flex items-center gap-2"
                    >
                      <Square size={12} fill="currentColor" /> Stop
                    </button>
                  )}
                </div>
              </div>

              <div
                className="flex-1 bg-[#0c0c0c] p-6 font-mono text-sm overflow-auto custom-scrollbar"
                onClick={() => document.getElementById('terminal-input')?.focus()}
              >
                <div className="flex flex-col min-h-full text-gray-300">
                  <div className="mb-4 text-gray-500 select-none">
                    MinicodeHub v2.0.0 [C++ Execution Environment]<br />
                    Type input below when prompted.
                  </div>

                  <div className="whitespace-pre-wrap break-words text-gray-300 leading-relaxed">
                    {video.terminalOutput}
                    <div ref={terminalEndRef} />
                  </div>

                  {video.isRunning && (
                    <div className="mt-2 flex items-center gap-2 group">
                      <span className="text-green-500 font-bold">➜</span>
                      <input
                        id="terminal-input"
                        type="text"
                        value={video.inputBuffer || ''}
                        onChange={(e) => setVideo(v => ({ ...v, inputBuffer: e.target.value }))}
                        onKeyDown={handleTerminalInput}
                        className="flex-1 bg-transparent border-none text-white focus:ring-0 p-0 font-mono caret-green-500 outline-none placeholder-gray-700"
                        placeholder="Type input here..."
                        autoComplete="off"
                        autoFocus
                      />
                    </div>
                  )}
                </div>
              </div>
            </div>
          )}
        </div>
      </div>
    </div>
  );
}

function App() {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/video/:id" element={<VideoDetail />} />
      </Routes>
    </Router>
  );
}

export default App;
