Job Market API Explorer
A full-stack web application that aggregates and analyzes job postings from multiple public job APIs, allowing users to search, filter, and visualize market trends by technology stack, location, salary, and more.

🚀 Features
Multi-API Integration: Fetches live job postings from APIs like Adzuna, Indeed API, and GitHub Jobs API.

Advanced Search & Filtering: Filter jobs by title, location, salary range, company, and tech stack.

Data Visualization: Charts showing most in-demand skills, average salary trends, and job posting volumes.

Real-Time Updates: Fetches the latest postings every time the user searches.

Responsive UI: Built for mobile, tablet, and desktop.

🛠 Tech Stack
Frontend: React.js + Tailwind CSS
Backend: Node.js + Express
Database: MongoDB (for caching and historical trend analysis)
APIs Used:

Adzuna API (Job search data)

GitHub Jobs API (Developer jobs)

[Optional] Indeed API
Other Services:

Chart.js for visualizations

Axios for HTTP requests


📦 Installation & Setup

# Clone the repository
git clone https://github.com/<your-username>/job-market-api-explorer.git

# Navigate into the project
cd job-market-api-explorer

# Install dependencies for both frontend and backend
cd client && npm install
cd ../server && npm install

# Create environment variables
# .env file in /server should include:
# ADZUNA_APP_ID=your_app_id
# ADZUNA_APP_KEY=your_app_key
# GITHUB_JOBS_API=https://jobs.github.com/positions.json

# Run backend
cd server
npm run dev

# Run frontend (in another terminal)
cd client
npm start


📊 Example API Response (Adzuna)

{
  "results": [
    {
      "title": "Software Engineer",
      "company": { "display_name": "BMO Financial Group" },
      "location": { "display_name": "Toronto, Ontario" },
      "salary_min": 80000,
      "salary_max": 120000,
      "redirect_url": "https://adzuna.com/..."
    }
  ]
}

🎯 Future Improvements
Add machine learning to classify jobs into broader categories.

Implement email alerts for new job postings matching saved searches.

Integrate LinkedIn API (if available).

🤝 Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
