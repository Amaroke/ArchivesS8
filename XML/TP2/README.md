# XPATH

### 1)

* /boutique/commande[datecommande='17/01/2007']

### 2)

* /boutique/commande[numerodecommande >= 2 and client = 'Bernard']

### 3)

* /boutique/commande[last()]

### 4)

* /boutique/commande[numerodecommande = 1]/*[not(self::produit)]

### 5)

* /boutique/commande[numerodecommande = 5]/client

### 6)

* /boutique/commande/client[@origine='particulier']

# XQUERY

### 7)

```
let $doc := doc("commande.xml")
return <nomclient>{$doc/boutique/commande[numerodecommande = 2]/client}</nomclient>
```

<nomclient><client origine="particulier">Jean-Marie</client></nomclient>

### 8)

```
for $b in
doc("commande.xml")//commande[1]/produit
let $al := $b/nom
return $al
```

```
<nom>Graveur * 16</nom>
<nom>DVD-R</nom>
```

### 9)

```
let $doc := doc("commande.xml")
for $c in $doc/boutique/commande[client="Bernard"]
return $c/produit/nom
```

```
<nom>Graveur * 16</nom>
<nom>DVD-R</nom>
<nom>Ecran 21 pouces</nom>
<nom>Clavier</nom>
```

### 10)

```
let $doc := doc("commande.xml")
for $c in distinct-values($doc/boutique/commande/client)
order by $c ascending
return $c
```

```
Bea
Bernard
Jean-Marie
```

### 11)

```
let $doc := doc("commande.xml")
return count(distinct-values($doc/boutique/commande[numerodecommande="1"]/produit/nom))
```

```
2
```

### 11bis)

```
let $doc := doc("commande.xml")
return count(distinct-values($doc/boutique/commande/produit/nom))
```

```
6
```

### 12)

```
for $b in
doc("commande.xml")//commande[1]
let $al := $b/produit
return <produit nombre="{count($al)}">
{ $al }
</produit>
```

```
<produit nombre="2">
    <produit>
        <nom>Graveur * 16</nom>
        <quantite>15</quantite>
        <pu>35</pu>
    </produit><produit>
        <nom>DVD-R</nom>
        <quantite>10</quantite>
        <pu>5</pu>
    </produit>
</produit>
```

### 13)

```
for $x in doc("commande.xml")//commande
return if($x/@type="papeterie")
then $x/client
else()
```

```
Jean-Marie
Bea
```

### 14)

```
for $x in doc(commande.xml)//commande
return if($x/@type="papeterie")
then <papeterie>{data($x/client)}</papeterie>
else <autre>{data($x/client)</autre>
```

```
<autre>Bernard</autre>
<papeterie>Jean-Marie</papeterie>
<autre>Bernard</autre>
<autre>Bernard</autre>
<papeterie>Bea</papeterie>
```

### 15)

```
let $doc := doc("commande.xml")
return 
  <html>
    <head>
      <title>Liste des clients</title>
    </head>
    <body>
      <h1>Liste des clients</h1>
      <ul>
        {distinct-values($doc/boutique/commande/client) ! <li>{.}</li>}
      </ul>
    </body>
  </html>
```

```
<html>
    <head>
        <title>Liste des clients</title>
    </head>
    <body>
        <h1>Liste des clients</h1>
            <ul>
                <li>Bernard</li>
                <li>Jean-Marie</li>
                <li>Bea</li>
            </ul>
    </body>
</html>
```

### 16)

```
for $livre in doc("livre.xml")//livre,
    $emprunt in doc("emprunt.xml")//contenu,
    $adherent in doc("adherent.xml")//membre
where $livre/isbn = $emprunt/isbn and $adherent/code = $emprunt/code
return <resultatrequete>
            <nom>{data($adherent/nom)}</nom>
            <isbn>{data($livre/isbn)}</isbn>
            <dateemprunt>{data($emprunt/dateemprunt)}</dateemprunt>
            <dateretour>{data($emprunt/dateretour)}</dateretour>
        </resultatrequete>
```

```
<resultatrequete><nom>Cardoni</nom><isbn>1234</isbn><dateemprunt>15/01/2007</dateemprunt><dateretour>21/01/2007</dateretour></resultatrequete>
<resultatrequete><nom>Kintzler</nom><isbn>1235</isbn><dateemprunt>19/01/2007</dateemprunt><dateretour/></resultatrequete>
<resultatrequete><nom>Kintzler</nom><isbn>12</isbn><dateemprunt>02/02/2007</dateemprunt><dateretour>10/02/2007</dateretour></resultatrequete>
<resultatrequete><nom>Lallement</nom><isbn>4567</isbn><dateemprunt>05/02/2007</dateemprunt><dateretour/></resultatrequete>
<resultatrequete><nom>Lallement</nom><isbn>89</isbn><dateemprunt>31/01/2007</dateemprunt><dateretour/></resultatrequete>
<resultatrequete><nom>Georgelin</nom><isbn>2345</isbn><dateemprunt>19/01/2007</dateemprunt><dateretour>25/01/2007</dateretour></resultatrequete>
<resultatrequete><nom>Cardoni</nom><isbn>567</isbn><dateemprunt>22/01/2007</dateemprunt><dateretour>24/01/2007</dateretour></resultatrequete>
<resultatrequete><nom>Lallement</nom><isbn>990</isbn><dateemprunt>22/01/2007</dateemprunt><dateretour/></resultatrequete>
```

### 17)

```
for $a in doc("adherent.xml")//membre,
    $e in doc("emprunt.xml")//contenu,
    $l in doc("livre.xml")//livre
where $a/code=$e/code and $e/isbn=$l/isbn
return <resultatrequete>
           <nom>{data($a/nom)}</nom>
           <prenom>{data($a/prenom)}</prenom>
           <titre>{data($l/titre)}</titre>
       </resultatrequete>
```

```
<resultatrequete><nom>Cardoni</nom><prenom>Jean-Marie</prenom><titre>ACCESS 20007</titre></resultatrequete>
<resultatrequete><nom>Cardoni</nom><prenom>Jean-Marie</prenom><titre>XHTML et CSS</titre></resultatrequete>
<resultatrequete><nom>Lallement</nom><prenom>Bernard</prenom><titre>Informatique de gestion</titre></resultatrequete>
<resultatrequete><nom>Lallement</nom><prenom>Bernard</prenom><titre>C# par la pratique</titre></resultatrequete>
<resultatrequete><nom>Lallement</nom><prenom>Bernard</prenom><titre>Vista configuration</titre></resultatrequete>
<resultatrequete><nom>Kintzler</nom><prenom>Agnès</prenom><titre>Débuter avec Visual studio 2005</titre></resultatrequete>
<resultatrequete><nom>Kintzler</nom><prenom>Agnès</prenom><titre>VB.net par la pratique</titre></resultatrequete>
<resultatrequete><nom>Georgelin</nom><prenom>Bèatrice</prenom><titre>XML introduction</titre></resultatrequete>
```