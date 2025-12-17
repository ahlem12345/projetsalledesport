from flask import Flask, request, jsonify
import os

app = Flask(__name__)

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
COACH_FILE = os.path.join(BASE_DIR, "../src/coach.txt")

@app.route("/stats", methods=["GET"])
def stats():
    id_recherche = request.args.get("id", type=int)

    total_coachs = 0
    total_cours = 0
    cours_prives = 0
    trouve = False

    with open(COACH_FILE, "r", encoding="latin-1") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue

            nom, prenom, idc, specialite, nbr_cours, coaching_prive = line.split(";")

            total_coachs += 1
            total_cours += int(nbr_cours)

            if coaching_prive.lower() == "oui":
                cours_prives += int(nbr_cours)

            if int(idc) == id_recherche:
                trouve = True

    if not trouve:
        return jsonify({"error": "Coach non trouvé"}), 404

    moyenne = total_cours / total_coachs if total_coachs else 0
    pourcentage = (cours_prives * 100 / total_cours) if total_cours else 0

    return jsonify({
        "total_coachs": total_coachs,
        "total_cours": total_cours,
        "cours_prives": cours_prives,
        "pourcentage_prives": round(pourcentage, 2),
        "moyenne_cours": round(moyenne, 2)
    })

if __name__ == "__main__":
    app.run(debug=True)

