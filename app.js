/*
Jo Yi Tham
Akshara Bharath
Emma Cai
Madison Tran
Fiona Mullins
Commander Schenk
Period 3
Compatibility Quiz — browser UI (mirrors bio.c + security.c)
*/

const QUESTIONS = 20;
const CHARACTERS = 15;
const MAX_DIFFERENCE = 4;
const MAX_PROFILES = 50;
const PRIVACY_KEY = "loveAtFirstWrite.privacy";
const REPORTS_KEY = "loveAtFirstWrite.reports";
const MAX_REASON = 100;

async function sendToServer(data) {
  try {
    const response = await fetch("/api/privacy", {
      //^sends informations from JavaScript to the Python server for processing
      method: "POST",
      //^tells the server that the request is a POST request, which means that data is being sent to the server
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify(data)
      //^converts the JavaScript object data into a JSON string and sends it as the body of the request
    });

    return await response.json();
    //^reads the response from the server and converts it back into a JavaScript object using response.json()
  } catch (error) {
    console.error("Could not connect to the server:", error);
    return { error: "Server connection failed" };
  }
}

const questions = [
  "I feel loved when my partner expresses appreciation verbally.",
  "Hearing that my partner is proud of me makes me feel secure.",
  "I feel closest to my partner when we spend uninterrupted time together.",
  "I value meaningful conversations without distractions.",
  "Thoughtful gifts make me feel appreciated.",
  "Small surprises make me feel remembered and valued.",
  "I feel loved when my partner helps me with tasks.",
  "I appreciate actions that make my day easier.",
  "Hugs and hand-holding help me feel connected to my partner.",
  "Physical affection is important in my relationships.",
  "I feel comfortable telling my partner what I need.",
  "I try to understand my partner's perspective during disagreements.",
  "I feel comfortable trusting my partner when we are apart.",
  "I believe healthy relationships require honesty and consistency.",
  "I value maintaining my own hobbies and friendships.",
  "Both partners should have personal space and time alone.",
  "I prefer discussing disagreements calmly rather than avoiding them.",
  "I am willing to compromise when my partner and I disagree.",
  "I feel comfortable sharing my fears and insecurities.",
  "I express my feelings rather than hide what bothers me."
];

const characters = [
  {
    name: "Peeta Mellark",
    source: "The Hunger Games",
    bio: "A kind and loyal tribute known for his compassion and courage.",
    traits: "Loyal, Caring, Brave",
    interests: "Baking, Painting, Helping Others",
    answers: [5, 5, 5, 5, 4, 5, 5, 5, 5, 4, 4, 5, 5, 5, 2, 3, 5, 5, 5, 5]
  },
  {
    name: "Hermione Granger",
    source: "Harry Potter",
    bio: "An intelligent witch who values knowledge and friendship.",
    traits: "Smart, Determined, Responsible",
    interests: "Reading, Learning, Problem Solving",
    answers: [4, 5, 4, 5, 3, 3, 4, 5, 3, 3, 4, 4, 4, 5, 5, 4, 4, 3, 3, 4]
  },
  {
    name: "Elizabeth Bennet",
    source: "Pride and Prejudice",
    bio: "A witty woman who values independence and personal growth.",
    traits: "Independent, Intelligent, Honest",
    interests: "Reading, Socializing, Walking",
    answers: [3, 3, 5, 5, 2, 2, 3, 3, 2, 2, 5, 3, 4, 5, 5, 4, 3, 3, 3, 5]
  },
  {
    name: "Mr. Darcy",
    source: "Pride and Prejudice",
    bio: "A reserved gentleman who forms deep and meaningful relationships.",
    traits: "Reserved, Loyal, Thoughtful",
    interests: "Reading, Estate Management, Travel",
    answers: [2, 3, 4, 5, 3, 3, 5, 5, 2, 2, 2, 3, 4, 5, 4, 5, 3, 3, 1, 2]
  },
  {
    name: "Romeo Montague",
    source: "Romeo and Juliet",
    bio: "A passionate romantic willing to do anything for love.",
    traits: "Romantic, Emotional, Impulsive",
    interests: "Poetry, Romance, Adventure",
    answers: [5, 4, 5, 4, 4, 4, 3, 2, 5, 5, 4, 2, 2, 4, 1, 1, 1, 2, 5, 5]
  },
  {
    name: "Sherlock Holmes",
    source: "Sherlock Holmes Stories",
    bio: "A brilliant detective with unmatched powers of deduction.",
    traits: "Analytical, Logical, Curious",
    interests: "Investigating, Violin, Solving Mysteries",
    answers: [2, 2, 3, 5, 1, 1, 3, 4, 1, 1, 2, 1, 5, 5, 5, 5, 3, 1, 1, 1]
  },
  {
    name: "Tony Stark",
    source: "Iron Man",
    bio: "A genius inventor who hides a caring heart behind confidence.",
    traits: "Confident, Intelligent, Creative",
    interests: "Technology, Engineering, Innovation",
    answers: [4, 5, 2, 2, 5, 5, 4, 4, 3, 3, 2, 2, 3, 3, 5, 4, 2, 2, 2, 1]
  },
  {
    name: "Jack Sparrow",
    source: "Pirates of the Caribbean",
    bio: "A clever pirate always looking for his next adventure.",
    traits: "Funny, Clever, Adventurous",
    interests: "Sailing, Exploring, Treasure Hunting",
    answers: [5, 4, 2, 1, 4, 4, 2, 2, 4, 3, 3, 2, 2, 1, 5, 5, 2, 3, 1, 1]
  },
  {
    name: "Shrek",
    source: "Shrek",
    bio: "An ogre who values loyalty, honesty, and friendship.",
    traits: "Loyal, Honest, Independent",
    interests: "Privacy, Nature, Family",
    answers: [2, 2, 4, 4, 2, 3, 4, 4, 3, 3, 2, 3, 4, 5, 5, 5, 2, 4, 2, 3]
  },
  {
    name: "Harry Potter",
    source: "Harry Potter",
    bio: "A brave wizard who is deeply loyal to his friends.",
    traits: "Brave, Loyal, Selfless",
    interests: "Quidditch, Adventure, Friendship",
    answers: [3, 4, 4, 4, 3, 3, 4, 4, 3, 3, 2, 4, 4, 5, 4, 3, 2, 4, 2, 2]
  },
  {
    name: "Katniss Everdeen",
    source: "The Hunger Games",
    bio: "A survivor who fiercely protects the people she loves.",
    traits: "Independent, Brave, Protective",
    interests: "Hunting, Archery, Family",
    answers: [1, 2, 3, 4, 2, 2, 5, 5, 2, 3, 1, 2, 2, 4, 5, 5, 2, 2, 1, 1]
  },
  {
    name: "Mario",
    source: "Super Mario Bros.",
    bio: "A heroic plumber known for helping others.",
    traits: "Friendly, Courageous, Helpful",
    interests: "Adventure, Sports, Saving Others",
    answers: [3, 3, 3, 2, 3, 4, 5, 5, 3, 3, 2, 4, 5, 5, 3, 3, 4, 4, 3, 3]
  },
  {
    name: "Darth Vader",
    source: "Star Wars",
    bio: "A powerful but conflicted figure seeking redemption.",
    traits: "Strong, Serious, Determined",
    interests: "Leadership, Strategy, Power",
    answers: [2, 4, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 1, 3, 4, 4, 1, 1, 1, 2]
  },
  {
    name: "Pikachu",
    source: "Pokemon",
    bio: "A cheerful Pokémon known for loyalty and friendship.",
    traits: "Energetic, Friendly, Loyal",
    interests: "Training, Friendship, Adventure",
    answers: [4, 4, 5, 5, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 2, 2, 3, 4, 4, 5]
  },
  {
    name: "Homer Simpson",
    source: "The Simpsons",
    bio: "A humorous father who deeply cares about his family.",
    traits: "Funny, Caring, Relaxed",
    interests: "Eating, Television, Family",
    answers: [4, 4, 4, 3, 5, 5, 2, 2, 5, 5, 3, 3, 4, 3, 4, 3, 2, 4, 4, 5]
  }
];

const names = characters.map((c) => c.name);

let settings = defaultPrivacy();
let answers = [];
let quizIndex = 0;
let scores = [];
let ranking = [];
let available = 0;
let currentMatch = 0;

function defaultPrivacy() {
  return {
    showBio: 1,
    showTraits: 1,
    showInterests: 1,
    showCompatibility: 1,
    blocked: Array(MAX_PROFILES).fill(0)
  };
}

function savePrivacy() {
  try {
    localStorage.setItem(
      PRIVACY_KEY,
      JSON.stringify({
        showBio: settings.showBio,
        showTraits: settings.showTraits,
        showInterests: settings.showInterests,
        showCompatibility: settings.showCompatibility,
        blocked: settings.blocked.slice(0, CHARACTERS)
      })
    );
    return 1;
  } catch (e) {
    console.warn("Warning: could not save your privacy settings.");
    return 0;
  }
}

function loadPrivacy() {
  try {
    const raw = localStorage.getItem(PRIVACY_KEY);
    if (!raw) {
      return 0;
    }

    const data = JSON.parse(raw);
    const loaded = defaultPrivacy();
    const values = [
      data.showBio,
      data.showTraits,
      data.showInterests,
      data.showCompatibility
    ];

    for (let i = 0; i < 4; i++) {
      if (values[i] !== 0 && values[i] !== 1) {
        console.warn("Saved privacy settings were damaged, so defaults were restored.");
        return 0;
      }
    }

    loaded.showBio = values[0];
    loaded.showTraits = values[1];
    loaded.showInterests = values[2];
    loaded.showCompatibility = values[3];

    if (!Array.isArray(data.blocked) || data.blocked.length < CHARACTERS) {
      console.warn("Saved privacy settings were damaged, so defaults were restored.");
      return 0;
    }

    for (let i = 0; i < CHARACTERS; i++) {
      const flag = data.blocked[i];
      if (flag !== 0 && flag !== 1) {
        console.warn("Saved privacy settings were damaged, so defaults were restored.");
        return 0;
      }
      loaded.blocked[i] = flag;
    }

    settings = loaded;
    return 1;
  } catch (e) {
    console.warn("Saved privacy settings were damaged, so defaults were restored.");
    return 0;
  }
}

function matchPercent(userAnswers, characterAnswers) {
  let totalDifference = 0;

  for (let i = 0; i < QUESTIONS; i++) {
    totalDifference += Math.abs(userAnswers[i] - characterAnswers[i]);
  }

  const maxDifference = QUESTIONS * MAX_DIFFERENCE;
  return (100.0 * (maxDifference - totalDifference)) / maxDifference;
}

function blockCharacter(index) {
  if (index < 0 || index >= CHARACTERS) {
    return;
  }

  settings.blocked[index] = 1;
  savePrivacy();

  sendToServer({
    action: "block",
    index: index
  }).then(result => {
    if (result.error) {
      console.warn("Could not block character in C: ", result.error);
    }
  });

  setStatus(`decision-status`, `${names[index]} has been blocked. They will not appear in your matches again.`);
}

function reportCharacter(index, reasonText) {
  if (index < 0 || index >= CHARACTERS) {
    return;
  }

  let cleaned = "";
  if (reasonText) {
    for (let i = 0; i < reasonText.length && cleaned.length < MAX_REASON; i++) {
      const code = reasonText.charCodeAt(i);
      if (code >= 32 && code !== 127) {
        cleaned += reasonText[i];
      }
    }
  }
  if (cleaned.length === 0) {
    cleaned = "No reason given";
  }

  sendToServer({
    action: "report",
    index: index,
    reason: cleaned
  }).then(result => {
    if (result.error) {
      console.warn("Could not save report to C: ", result.error);
    }
  });

  try {
    const existing = JSON.parse(localStorage.getItem(REPORTS_KEY) || "[]");
    existing.push(`Character: ${names[index]} | Reason: ${cleaned}`);
    localStorage.setItem(REPORTS_KEY, JSON.stringify(existing));
    setStatus("decision-status", `Thank you. Your report about ${names[index]} has been recorded.`);
  } catch (e) {
    setStatus("decision-status", "Warning: your report could not be saved.");
  }

  blockCharacter(index);
}

function showScreen(id) {
  document.querySelectorAll(".screen").forEach((el) => el.classList.add("hidden"));
  const screen = document.getElementById(`screen-${id}`);
  if (screen) {
    screen.classList.remove("hidden");
  }
}

function setStatus(elementId, text) {
  const el = document.getElementById(elementId);
  if (el) {
    el.textContent = text || "";
  }
}

function shownLabel(flag) {
  return flag ? "SHOWN" : "HIDDEN";
}

function renderPrivacyToggles() {
  const container = document.getElementById("privacy-toggles");
  container.innerHTML = "";

  const items = [
    { key: "showBio", label: "1: Biography on profiles" },
    { key: "showTraits", label: "2: Personality traits" },
    { key: "showInterests", label: "3: Interests" },
    { key: "showCompatibility", label: "4: Compatibility percentages" }
  ];

  items.forEach((item) => {
    const btn = document.createElement("button");
    btn.type = "button";
    btn.textContent = `${item.label}:       ${shownLabel(settings[item.key])}`;
    btn.addEventListener("click", async () => {
      settings[item.key] = settings[item.key] ? 0 : 1;

      savePrivacy();
      renderPrivacyToggles();

      const result = await sendToServer({
        action: "set",
        setting: item.key,
        value: settings[item.key]
      });

      if (result.error) {
        console.error("Could not save setting to C: ", result.error);
      }
    });
    container.appendChild(btn);
  });
}

function renderBlockedList(forUnblock) {
  const box = document.getElementById("blocked-list");
  box.classList.remove("hidden");
  box.innerHTML = "<strong>Blocked characters:</strong>";

  let blockedCount = 0;
  const list = document.createElement("ul");

  for (let i = 0; i < CHARACTERS; i++) {
    if (settings.blocked[i]) {
      blockedCount++;
      const li = document.createElement("li");
      li.textContent = `${i + 1}. ${names[i]}`;

      if (forUnblock) {
        const unblockBtn = document.createElement("button");
        unblockBtn.type = "button";
        unblockBtn.className = "inline-btn";
        unblockBtn.textContent = "Unblock";

        unblockBtn.addEventListener("click", () => {
          settings.blocked[i] = 0;
          savePrivacy();

          sendToServer({
            action: "unblock",
            index: i
        }).then(result => {
          if (result.error) {
            console.warn("Could not unblock character in C: ", result.error);
          }
        });

        renderBlockedList(true);

      });

        li.appendChild(document.createTextNode(" "));
        li.appendChild(unblockBtn);
      }

      list.appendChild(li);
    }
  }

  if (blockedCount === 0) {
    box.innerHTML += "<p>You have not blocked anyone.</p>";
  } else {
    box.appendChild(list);
  }
}

function startQuiz() {
  answers = [];
  quizIndex = 0;
  showQuizQuestion();
  showScreen("quiz");
}

function showQuizQuestion() {
  document.getElementById("quiz-number").textContent = String(quizIndex + 1);
  document.getElementById("quiz-question").textContent = questions[quizIndex];
  const pct = (quizIndex / QUESTIONS) * 100;
  document.getElementById("quiz-progress").style.width = `${pct}%`;
}

function recordAnswer(value) {
  answers[quizIndex] = value;
  quizIndex++;

  if (quizIndex < QUESTIONS) {
    showQuizQuestion();
    return;
  }

  document.getElementById("quiz-progress").style.width = "100%";
  computeMatches();
}

function computeMatches() {
  scores = [];
  ranking = [];
  available = 0;

  for (let i = 0; i < CHARACTERS; i++) {
    scores[i] = matchPercent(answers, characters[i].answers);
    if (!settings.blocked[i]) {
      ranking[available] = i;
      available++;
    }
  }

  if (available === 0) {
    document.getElementById("empty-message").textContent =
      "Every character is blocked, so there are no matches to show. Unblock someone in Privacy and safety settings to see matches.";
    showScreen("empty");
    return;
  }

  for (let i = 0; i < available - 1; i++) {
    for (let j = 0; j < available - 1 - i; j++) {
      if (scores[ranking[j]] < scores[ranking[j + 1]]) {
        const temp = ranking[j];
        ranking[j] = ranking[j + 1];
        ranking[j + 1] = temp;
      }
    }
  }

  renderMatchesList();
  showScreen("matches");
}

function renderMatchesList() {
  const list = document.getElementById("matches-list");
  list.innerHTML = "";

  for (let i = 0; i < available; i++) {
    const c = ranking[i];
    const li = document.createElement("li");
    if (settings.showCompatibility) {
      li.textContent = `${characters[c].name} (${characters[c].source}): ${scores[c].toFixed(1)}%`;
    } else {
      li.textContent = `${characters[c].name} (${characters[c].source})`;
    }
    list.appendChild(li);
  }

  const top = characters[ranking[0]];
  const topEl = document.getElementById("top-match");
  if (settings.showCompatibility) {
    topEl.textContent = `Your perfect match is ${top.name} from ${top.source}!`;
  } else {
    topEl.textContent = `Your top match is ${top.name} from ${top.source}!`;
  }
}

function beginDecisions() {
  currentMatch = 0;
  setStatus("decision-status", "");
  showCurrentDecision();
  showScreen("decision");
}

function showCurrentDecision() {
  if (currentMatch >= available) {
    document.getElementById("empty-message").textContent = "No match was selected.";
    showScreen("empty");
    return;
  }

  // Skip characters blocked during this decision loop
  while (currentMatch < available && settings.blocked[ranking[currentMatch]]) {
    currentMatch++;
  }

  if (currentMatch >= available) {
    document.getElementById("empty-message").textContent = "No match was selected.";
    showScreen("empty");
    return;
  }

  const c = ranking[currentMatch];
  document.getElementById("decision-name").textContent =
    `Suggested match: ${characters[c].name} from ${characters[c].source}`;

  const compat = document.getElementById("decision-compat");
  if (settings.showCompatibility) {
    compat.textContent = `Compatibility: ${scores[c].toFixed(1)}%`;
    compat.classList.remove("hidden");
  } else {
    compat.textContent = "";
    compat.classList.add("hidden");
  }
}

function handleDecision(decision) {
  const c = ranking[currentMatch];

  if (decision === "A") {
    showProfile(characters[c], scores[c]);
    return;
  }

  if (decision === "B") {
    blockCharacter(c);
  } else if (decision === "F") {
    document.getElementById("report-reason").value = "";
    showScreen("report");
    return;
  } else {
    setStatus("decision-status", `You rejected ${characters[c].name} from ${characters[c].source}.`);
  }

  advanceMatch();
}

function advanceMatch() {
  currentMatch++;

  if (currentMatch < available) {
    while (currentMatch < available && settings.blocked[ranking[currentMatch]]) {
      currentMatch++;
    }
  }

  if (currentMatch >= available) {
    document.getElementById("empty-message").textContent = "No match was selected.";
    showScreen("empty");
    return;
  }

  const next = ranking[currentMatch];
  setStatus(
    "decision-status",
    `Moving to your next best match... Next match: ${characters[next].name} from ${characters[next].source}`
  );
  showCurrentDecision();
  showScreen("decision");
}

function showProfile(c, compatibility) {
  const box = document.getElementById("profile-content");
  let html = `<p><strong>NAME:</strong> ${c.name}</p>`;
  html += `<p><strong>SOURCE:</strong> ${c.source}</p>`;

  if (settings.showBio) {
    html += `<p><strong>BIOGRAPHY:</strong><br>${c.bio}</p>`;
  }
  if (settings.showTraits) {
    html += `<p><strong>PERSONALITY TRAITS:</strong><br>${c.traits}</p>`;
  }
  if (settings.showInterests) {
    html += `<p><strong>INTERESTS:</strong><br>${c.interests}</p>`;
  }
  if (settings.showCompatibility) {
    html += `<p><strong>COMPATIBILITY:</strong> ${compatibility.toFixed(1)}%</p>`;
  }

  box.innerHTML = html;
  showScreen("profile");
}

function wireEvents() {
  document.body.addEventListener("click", (event) => {
    const target = event.target.closest("button");
    if (!target) {
      return;
    }

    const action = target.getAttribute("data-action");
    const answer = target.getAttribute("data-answer");
    const decision = target.getAttribute("data-decision");

    if (answer) {
      recordAnswer(Number(answer));
      return;
    }

    if (decision) {
      handleDecision(decision);
      return;
    }

    if (!action) {
      return;
    }

    switch (action) {
      case "start-quiz":
        startQuiz();
        break;
      case "open-privacy":
        document.getElementById("blocked-list").classList.add("hidden");
        renderPrivacyToggles();
        showScreen("privacy");
        break;
      case "open-notice":
        showScreen("notice");
        break;
      case "quit":
        showScreen("quit");
        break;
      case "back-menu":
        showScreen("menu");
        break;
      case "view-blocked":
        renderBlockedList(false);
        break;
      case "unblock-prompt":
        renderBlockedList(true);
        break;
      case "reset-visibility":
        settings.showBio = 1;
        settings.showTraits = 1;
        settings.showInterests = 1;
        settings.showCompatibility = 1;
        savePrivacy();

        ["showBio", "showTraits", "showInterests", "showCompatibility"].forEach(setting => {
          sendToServer({
            action: "set",
            setting: setting,
            value: 1
          });
        });

        renderPrivacyToggles();
        break;
      case "begin-decisions":
        beginDecisions();
        break;
      case "submit-report": {
        const c = ranking[currentMatch];
        const reason = document.getElementById("report-reason").value;

        reportCharacter(c, reason);
        advanceMatch();
        break;
      }

      case "cancel-report":
        showScreen("decision");
        break;
      default:
        break;
    }
  });
}

function init() {
  if (!loadPrivacy()) {
    settings = defaultPrivacy();
  }

  wireEvents();
  showScreen("menu");
}

init();
