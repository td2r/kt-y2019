import random

nucleotides = ['A', 'C', 'G', 'T']

def select_random_motifs(dnas, k):
    motifs = []
    for dna in dnas:
        pos = random.randint(0, len(dna) - k)
        motifs.append(dna[pos : pos + k])
    return motifs

def motifs_score(motifs):
    n = len(motifs)
    k = len(motifs[0])
    sub = 0
    for i in range(k):
        cnt = {nuc: 0 for nuc in nucleotides}
        for j in range(n):
            cnt[motifs[j][i]] += 1
        sub += max(cnt.values())
    return n * k - sub

def motifs_profile(motifs):
    k = len(motifs[0])
    profile = {nuc: [0] * k for nuc in nucleotides}
    for i in range(k):
        for motif in motifs:
            profile[motif[i]][i] += 1
        for nuc in nucleotides:
            profile[nuc][i] = (profile[nuc][i] + 1) / (len(motifs) + 4) # pseudocounts
    return profile

def motifs_from_profile(profile, dnas):
    k = len(profile[nucleotides[0]])
    l = len(dnas[0])
    motifs = []
    for dna in dnas:
        max_pr = -1
        motif_pos = -1
        for j in range(l - k + 1):
            cur_pr = 1
            for t in range(k):
                cur_pr *= profile[dna[j + t]][t]
            if cur_pr > max_pr:
                max_pr = cur_pr
                motif_pos = j
        motifs.append(dna[motif_pos : motif_pos + k])
    return motifs

def randomized_motif_search(dna, k):
    best_motifs = select_random_motifs(dna, k)
    best_score = motifs_score(best_motifs)
    while True:
        profile = motifs_profile(best_motifs)
        motifs = motifs_from_profile(profile, dna)
        score = motifs_score(motifs)
        if score < best_score:
            best_score = score
            best_motifs = motifs
        else:
            return (best_score, best_motifs)

# motifs = [
#         "TCGGGGGTTTTT",
#         "CCGGTGACTTAC",
#         "ACGGGGATTTTC",
#         "TTGGGGACTTTT",
#         "AAGGGGACTTCC",
#         "TTGGGGACTTCC",
#         "TCGGGGATTCAT",
#         "TCGGGGATTCCT",
#         "TAGGGGAACTAC",
#         "TCGGGTATAACC"
#         ]
#
# print(motifs_score(motifs))
# profile = motifs_profile(motifs)
# for k, v in profile.items():
#     print(str(k) + ': ' + str(v))

################################################################

# profile = {
#         'A': [4/5, 0.0, 0.0, 1/5],
#         'C': [0, 3/5, 1/5, 0],
#         'G': [1/5, 1/5, 4/5, 0.0],
#         'T': [0, 1/5, 0, 4/5],
#         }
# dna = [
#         "TTACCTTAAC",
#         "GATGTCTGTC",
#         "ACGGCGTTAG",
#         "CCCTAACGAG",
#         "CGTCAGAGGT",
#         ]
#
# motifs = motifs_from_profile(profile, dna)
# for motif in motifs:
#     print(motif)

################################################################

(k, t) = map(int, input().split())
dna = [input() for _ in range(t)]
(best_score, best_motifs) = randomized_motif_search(dna, k)
for i in range(1000):
    print('Attempt ' + str(i + 1))
    (score, motifs) = randomized_motif_search(dna, k)
    if score < best_score:
        (best_score, best_motifs) = (score, motifs)

print(best_score)
for motif in best_motifs:
    print(motif)
