import { useEffect } from 'react';
import { useLocation } from 'react-router-dom';

export default function ScrollToAnchor() {
    const location = useLocation();

    useEffect(() => {
        if (location.hash) {
            let elem = document.getElementById(location.hash.slice(1));
            if (elem) {
                elem.scrollIntoView({ behavior: 'smooth' });
            } else {
                // Try again a bit later in case the component hasn't rendered yet
                setTimeout(() => {
                    let elemRetry = document.getElementById(location.hash.slice(1));
                    if (elemRetry) {
                        elemRetry.scrollIntoView({ behavior: 'smooth' });
                    }
                }, 100);
            }
        } else {
            window.scrollTo({ top: 0, left: 0, behavior: 'instant' });
        }
    }, [location]);

    return null;
}
